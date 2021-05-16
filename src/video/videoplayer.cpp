#include "videoplayer.h"
#include "../log.h"
#include <utility>
#include "../profile/game.h"

namespace Impacto {
namespace Video {

int StreamRead(void* ptr, uint8_t* buf, int buf_size) {
  Io::InputStream* stream = reinterpret_cast<Io::InputStream*>(ptr);

  uint64_t bytesRead = stream->Read(buf, buf_size);
  if ((bytesRead == IoError_Fail || bytesRead == IoError_Eof)) return AVERROR_EOF;

  return bytesRead;
}

int64_t StreamSeek(void* ptr, int64_t pos, int origin) {
  Io::InputStream* stream = reinterpret_cast<Io::InputStream*>(ptr);

  if (origin == AVSEEK_SIZE) return stream->Meta.Size;
  int64_t newPos = stream->Seek(pos, origin);
  if ((newPos == IoError_Fail) || (newPos == IoError_Eof)) return AVERROR_EOF;

  return newPos;
}

static int PlayerRead(void* ptr) {
  ((VideoPlayer*)ptr)->Read();
  return 0;
}

static int PlayerDecodeVideo(void* ptr) {
  ((VideoPlayer*)ptr)->Decode(AVMEDIA_TYPE_VIDEO);
  return 0;
}

static int PlayerDecodeAudio(void* ptr) {
  ((VideoPlayer*)ptr)->Decode(AVMEDIA_TYPE_AUDIO);
  return 0;
}
static int PlayerAudio(void* ptr) {
  ((VideoPlayer*)ptr)->ProcessAudio();
  return 0;
}

Clock::Clock() {
  Speed = 1.0;
  Paused = false;
  Pts = NAN;
  LastUpdated = av_gettime_relative() / 1000000.0;
  PtsDrift = Pts - LastUpdated;
  Serial = -1;
}

void Clock::SyncTo(Clock* target) {
  double clock = Get();
  double targetClock = target->Get();
  if (!isnan(targetClock) && (isnan(clock) || fabs(clock - targetClock) > 10.0))
    Set(targetClock, target->Serial);
}

void Clock::Set(double pts, int serial) {
  double time = av_gettime_relative() / 1000000.0;
  Pts = pts;
  LastUpdated = time;
  PtsDrift = Pts - time;
  Serial = serial;
}

double Clock::Get() {
  if (Paused) {
    return Pts;
  } else {
    double time = av_gettime_relative() / 1000000.0;
    return PtsDrift + time - (time - LastUpdated) * (1.0 - Speed);
  }
}

VideoPlayer::~VideoPlayer() { IsInit = false; }

void VideoPlayer::Init() {
  assert(IsInit == false);

  // Testing crap
  alGenSources(1, &ALSource);
  alSourcef(ALSource, AL_PITCH, 1);
  alSourcef(ALSource, AL_GAIN, 1);
  alSource3f(ALSource, AL_POSITION, 0, 0, 0);
  alSource3f(ALSource, AL_VELOCITY, 0, 0, 0);
  alSourcei(ALSource, AL_LOOPING, AL_FALSE);

  alGenBuffers(AudioBufferCount, BufferIds);

  Lock = SDL_CreateMutex();

  IsInit = true;
}

void VideoPlayer::Play(Io::InputStream* stream, bool looping) {
  // Don't do anything if we don't have the video system
  if (!IsInit) return;
  ImpLog(LL_Trace, LC_Video, "Opening file: %s from: %s\n",
         stream->Meta.FileName.c_str(), stream->Meta.ArchiveFileName.c_str());

  int videoStreamId = -1;
  int audioStreamId = -1;
  FileStreamBuffer = (uint8_t*)av_malloc(FILESTREAMBUFFERSZ);
  if (FileStreamBuffer == nullptr) {
    ImpLog(LL_Error, LC_Video, "Error allocating buffer!\n");
    return;
  }
  IoContext = avio_alloc_context(FileStreamBuffer, FILESTREAMBUFFERSZ, 0,
                                 stream, StreamRead, 0, StreamSeek);

  FormatContext = avformat_alloc_context();
  FormatContext->pb = IoContext;
  FormatContext->flags |= AVFMT_FLAG_CUSTOM_IO;

  if (avformat_open_input(&FormatContext, "", nullptr, nullptr) < 0) {
    ImpLog(LL_Error, LC_Video, "Error opening file!\n");
    return;
  }

  avformat_find_stream_info(FormatContext, nullptr);

  videoStreamId = av_find_best_stream(FormatContext, AVMEDIA_TYPE_VIDEO, -1, -1,
                                      nullptr, 0);
  audioStreamId = av_find_best_stream(FormatContext, AVMEDIA_TYPE_AUDIO, -1,
                                      videoStreamId, nullptr, 0);
  AVStream* videoStream = FormatContext->streams[videoStreamId];
  AVStream* audioStream = NULL;
  if (audioStreamId != AVERROR_STREAM_NOT_FOUND)
    audioStream = FormatContext->streams[audioStreamId];

  for (int j = 0; j < FormatContext->nb_streams; ++j) {
    FormatContext->streams[j]->discard =
        j == videoStreamId || j == audioStreamId ? AVDISCARD_DEFAULT
                                                 : AVDISCARD_ALL;
  }

  AVCodec* codec = NULL;
  // Find and open the video codec
  if (videoStreamId != AVERROR_STREAM_NOT_FOUND) {
    codec = avcodec_find_decoder(videoStream->codecpar->codec_id);
    if (codec == NULL) {
      ImpLog(LL_Error, LC_Video, "Unsupported codec!\n");
      return;
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, videoStream->codecpar);

    avcodec_open2(codecCtx, codec, nullptr);
    VideoStream = new FFmpegStream(videoStream, codecCtx);
    Test.Init(codecCtx->width, codecCtx->height);
  }

  // Find and open the audio codec
  if (audioStreamId != AVERROR_STREAM_NOT_FOUND) {
    codec = avcodec_find_decoder(audioStream->codecpar->codec_id);
    if (codec == NULL) {
      ImpLog(LL_Error, LC_Video, "Unsupported codec!\n");
      return;
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecCtx, audioStream->codecpar);

    avcodec_open2(codecCtx, codec, nullptr);
    AudioStream = new FFmpegStream(audioStream, codecCtx);
    AudioStream->Duration = audioStream->duration *
                            av_q2d(audioStream->time_base) *
                            codecCtx->sample_rate;

    AudioConvertContext = swr_alloc_set_opts(
        NULL, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, codecCtx->sample_rate,
        AV_CH_LAYOUT_STEREO, codecCtx->sample_fmt, codecCtx->sample_rate, 0,
        NULL);
    swr_init(AudioConvertContext);
    av_samples_alloc_array_and_samples(&AudioBuffer, &AudioLinesize,
                                       codecCtx->channels, 32000,
                                       AV_SAMPLE_FMT_S16, 0);
  }

  VideoClock = new Clock();
  ExternalClock = new Clock();
  AudioClock = new Clock();

  MasterClock = ExternalClock;

  MaxFrameDuration =
      (FormatContext->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;

  IsPlaying = true;

  // Danger zone
  SDL_CreateThread(&PlayerRead, "videoplayer::read", this);
  if (VideoStream) {
    SDL_CreateThread(&PlayerDecodeVideo, "videoplayer::decodevideo", this);
  }
  if (AudioStream) {
    MasterClock = AudioClock;
    SDL_CreateThread(&PlayerDecodeAudio, "videoplayer::decodeaudio", this);
    SDL_CreateThread(&PlayerAudio, "videoplayer::audio", this);
  }
}

void VideoPlayer::Read() {
  int serial = 0;
  AVPacket* packet = av_packet_alloc();
  if (!packet) {
    ImpLog(LL_Error, LC_Video, "Failed to allocate a packet!\n");
    return;
  }
  while (true) {
    if (VideoStream->PacketQueue.size() > 20) {
      SDL_LockMutex(VideoStream->PacketLock);
      SDL_CondWaitTimeout(VideoStream->ReadCond, VideoStream->PacketLock, 10);
      SDL_UnlockMutex(VideoStream->PacketLock);
      continue;
    }
    int ret = av_read_frame(FormatContext, packet);
    if (ret >= 0) {
      if (packet->stream_index == VideoStream->stream->index) {
        AVPacketItem item;
        item.Packet = av_packet_clone(packet);
        item.Serial = serial++;
        VideoStream->PacketQueue.push(item);
      } else if (packet->stream_index == AudioStream->stream->index) {
        AVPacketItem item;
        item.Packet = av_packet_clone(packet);
        item.Serial = serial++;
        SDL_LockMutex(AudioStream->PacketLock);
        AudioStream->PacketQueue.push(item);
        SDL_UnlockMutex(AudioStream->PacketLock);
      } else {
        av_packet_unref(packet);
      }
    } else {
      if (ret == AVERROR_EOF) {
        ImpLog(LL_Debug, LC_Video, "EOF!\n");
        AVPacketItem item;
        item.Packet = av_packet_clone(packet);
        item.Serial = INT32_MIN;
        if (AudioStream) AudioStream->PacketQueue.push(item);
        VideoStream->PacketQueue.push(item);
      } else {
        char error[255];
        av_strerror(ret, error, 255);
        ImpLog(LL_Error, LC_Video, "Uh oh %s\n", error);
      }
      av_packet_unref(packet);
      break;
    }
  }
}

void VideoPlayer::Decode(AVMediaType avType) {
  int ret = 0;
  AVFrame* frame = av_frame_alloc();
  FFmpegStream* stream = 0;
  switch (avType) {
    case AVMEDIA_TYPE_VIDEO:
      stream = VideoStream;
      break;
    case AVMEDIA_TYPE_AUDIO:
      stream = AudioStream;
      break;
  }

  while (true) {
    SDL_LockMutex(stream->PacketLock);
    if (stream->PacketQueue.empty()) {
      SDL_CondSignal(VideoStream->ReadCond);
      SDL_UnlockMutex(stream->PacketLock);
      continue;
    }
    SDL_UnlockMutex(stream->PacketLock);
    if (stream->FrameQueue.size() > 100) {
      SDL_LockMutex(stream->FrameLock);
      SDL_CondWait(stream->DecodeCond, stream->FrameLock);
      SDL_UnlockMutex(stream->FrameLock);
      continue;
    }
    SDL_LockMutex(stream->PacketLock);
    auto packet = stream->PacketQueue.front();
    SDL_UnlockMutex(stream->PacketLock);
    do {
      ret = avcodec_receive_frame(stream->CodecContext, frame);
      if (ret >= 0) {
        AVFrameItem item;
        item.Frame = av_frame_clone(frame);
        item.Serial = 0;
        item.Timestamp =
            frame->best_effort_timestamp * av_q2d(stream->stream->time_base);

        SDL_LockMutex(stream->FrameLock);
        stream->FrameQueue.push(item);
        SDL_UnlockMutex(stream->FrameLock);
        av_frame_unref(frame);
      }
      if (ret == AVERROR_EOF) {
        AVFrameItem item;
        item.Frame = av_frame_clone(frame);
        item.Serial = INT32_MIN;
        SDL_LockMutex(stream->FrameLock);
        stream->FrameQueue.push(item);
        SDL_UnlockMutex(stream->FrameLock);
        av_frame_unref(frame);
        avcodec_flush_buffers(stream->CodecContext);
        return;
      }
    } while (ret != AVERROR(EAGAIN));

    if (packet.Serial == INT32_MIN) {
      AVFrameItem item;
      item.Frame = 0;
      item.Serial = INT32_MIN;
      SDL_LockMutex(stream->FrameLock);
      stream->FrameQueue.push(item);
      SDL_UnlockMutex(stream->FrameLock);
      return;
    }
    if (packet.Packet->data) {
      ret = avcodec_send_packet(stream->CodecContext, packet.Packet);
      if (ret >= 0) {
        av_packet_free(&packet.Packet);
        SDL_LockMutex(stream->PacketLock);
        stream->PacketQueue.pop();
        SDL_UnlockMutex(stream->PacketLock);
      }
    }
  }
}

void VideoPlayer::Stop() {
  IsPlaying = false;
  if (AudioBuffer) av_free(AudioBuffer);
  FrameTimer = 0.0;
  PreviousFrameTimestamp = 0.0;
  delete VideoStream;
  delete AudioStream;
  avformat_close_input(&FormatContext);
  reinterpret_cast<Io::InputStream*>(IoContext->opaque)->~InputStream();
  avio_context_free(&IoContext);
}

void VideoPlayer::ProcessAudio() {
  while (IsPlaying) {
    // Audio
    SDL_LockMutex(AudioStream->FrameLock);
    // ImpLog(LL_Trace, LC_Video, "AudioStream->FrameQueue.size() = %d\n",
    //       AudioStream->FrameQueue.size());
    if (!AudioStream->FrameQueue.empty()) {
      SDL_UnlockMutex(AudioStream->FrameLock);
      alGetSourcei(ALSource, AL_BUFFERS_PROCESSED, &FreeBufferCount);
      if (First) {
        FreeBufferCount = AudioBufferCount;
        First = false;
      }

      int currentlyPlayingBuffer =
          (FirstFreeBuffer + FreeBufferCount) % AudioBufferCount;

      int offset;
      alGetSourcei(ALSource, AL_SAMPLE_OFFSET, &offset);
      int samplePosition =
          BufferStartPositions[currentlyPlayingBuffer] + offset;
      samplePosition = std::min(samplePosition, AudioStream->Duration);
      double position =
          samplePosition / (double)AudioStream->CodecContext->sample_rate;
      // ImpLog(LL_Debug, LC_Video, "samplePosition: %f\n", position);

      AudioClock->Set(position, 0);

      FillAudioBuffers();
      ALint sourceState;
      alGetSourcei(ALSource, AL_SOURCE_STATE, &sourceState);
      if (sourceState == AL_STOPPED || sourceState == AL_INITIAL) {
        alSourcePlay(ALSource);
      }
    } else {
      SDL_UnlockMutex(AudioStream->FrameLock);
      ImpLog(LL_Debug, LC_Video, "Ran out of audio frames!\n");
    }
  }
}

void VideoPlayer::Update(float dt) {
  if (IsPlaying) {
    SDL_LockMutex(VideoStream->FrameLock);
    while (!VideoStream->FrameQueue.empty()) {
      // ImpLog(LL_Trace, LC_Video, "VideoStream->FrameQueue.size() = %d\n",
      //       VideoStream->FrameQueue.size());
      auto frame = VideoStream->FrameQueue.front();
      SDL_UnlockMutex(VideoStream->FrameLock);
      if (frame.Serial == INT32_MIN) {
        Stop();
        return;
      }
      if (!FrameTimer) {
        FrameTimer = av_gettime_relative() / 1000000.0;
      }
      double time = av_gettime_relative() / 1000000.0;
      double duration = frame.Timestamp - PreviousFrameTimestamp;

      duration = GetTargetDelay(duration);

      if (time < FrameTimer + duration) {
        break;
      }

      FrameTimer += duration;
      PreviousFrameTimestamp = frame.Timestamp;

      VideoClock->Set(frame.Timestamp, frame.Serial);
      ExternalClock->SyncTo(VideoClock);

      Test.Submit(frame.Frame->data[0], frame.Frame->data[1],
                  frame.Frame->data[2]);
      av_frame_free(&frame.Frame);
      SDL_LockMutex(VideoStream->FrameLock);
      VideoStream->FrameQueue.pop();
      if (VideoStream->FrameQueue.size() < 10) {
        SDL_CondSignal(VideoStream->DecodeCond);
      }
    }
    SDL_UnlockMutex(VideoStream->FrameLock);
  }
}

void VideoPlayer::Render() {
  if (IsPlaying) {
    float widthScale = Profile::DesignWidth / Test.Width;
    float heightScale = Profile::DesignHeight / Test.Height;
    Renderer2D::DrawVideoTexture(Test, glm::vec2(0.0f, 0.0f), glm::vec4(1.0f),
                                 glm::vec2(widthScale, heightScale));
  }
}

double VideoPlayer::GetTargetDelay(double duration) {
  double diff = VideoClock->Get() - MasterClock->Get();
  double sync_threshold = std::max(0.04, std::min(0.1, duration));
  if (!isnan(diff) && fabs(diff) < MaxFrameDuration) {
    if (diff <= -sync_threshold)
      duration = std::max(0.0, duration + diff);
    else if (diff >= sync_threshold && duration > 0.1)
      duration = duration + diff;
    else if (diff >= sync_threshold)
      duration = 2 * duration;
  }
  ImpLog(LL_Trace, LC_Video, "Target delay: %f\n", duration);

  return duration;
}

void VideoPlayer::FillAudioBuffers() {
  while (FreeBufferCount) {
    int totalSize = 0;

    do {
      bool firstFrame = true;

      SDL_LockMutex(AudioStream->FrameLock);
      if (AudioStream->FrameQueue.empty()) {
        SDL_UnlockMutex(AudioStream->FrameLock);
        continue;
      }
      auto aFrame = AudioStream->FrameQueue.front();
      SDL_UnlockMutex(AudioStream->FrameLock);

      if (aFrame.Serial == INT32_MIN) {
        SDL_LockMutex(AudioStream->FrameLock);
        AudioStream->FrameQueue.pop();
        SDL_UnlockMutex(AudioStream->FrameLock);
        break;
      }

      if (firstFrame) {
        BufferStartPositions[FirstFreeBuffer] =
            aFrame.Frame->best_effort_timestamp *
            av_q2d(AudioStream->stream->time_base) * aFrame.Frame->sample_rate;
        firstFrame = false;
      }

      int64_t delay =
          swr_get_delay(AudioConvertContext, aFrame.Frame->sample_rate);
      int64_t samplesPerCh = av_rescale_rnd(
          (int64_t)aFrame.Frame->nb_samples + delay, aFrame.Frame->sample_rate,
          aFrame.Frame->sample_rate, AV_ROUND_UP);
      int outputSamples =
          swr_convert(AudioConvertContext, AudioBuffer, samplesPerCh,
                      (const uint8_t**)aFrame.Frame->extended_data,
                      aFrame.Frame->nb_samples);

      int bufferSize = av_samples_get_buffer_size(NULL, 2, outputSamples,
                                                  AV_SAMPLE_FMT_S16, 1);
      memcpy(&HostBuffer[totalSize], AudioBuffer[0], bufferSize);
      totalSize += bufferSize;

      av_frame_free(&aFrame.Frame);
      SDL_LockMutex(AudioStream->FrameLock);
      AudioStream->FrameQueue.pop();
      if (AudioStream->FrameQueue.size() < 10) {
        SDL_CondSignal(AudioStream->DecodeCond);
      }
      SDL_UnlockMutex(AudioStream->FrameLock);

    } while (totalSize <= 4096);

    alSourceUnqueueBuffers(ALSource, 1, &BufferIds[FirstFreeBuffer]);
    FreeBufferCount--;

    alBufferData(BufferIds[FirstFreeBuffer], AL_FORMAT_STEREO16, HostBuffer,
                 totalSize, AudioStream->CodecContext->sample_rate);

    alSourceQueueBuffers(ALSource, 1, &BufferIds[FirstFreeBuffer]);

    FirstFreeBuffer++;
    FirstFreeBuffer %= AudioBufferCount;
  }
}

}  // namespace Video
}  // namespace Impacto
