root.Fonts = {
    "Default": {
        Type: FontType.Basic,
        Sheet: "Font",
        Columns: 64,
        Rows: 39,
        Widths: new Array(64 * 39),
        DesignColWidth: 32,
        LineSpacing: 22,
    }
};

var westernWidths = [
    0x20, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x13, 0x0B, 0x13, 0x14,
    0x14, 0x12, 0x12, 0x13, 0x14, 0x13, 0x1A, 0x15, 0x17, 0x17, 0x14, 0x14,
    0x18, 0x18, 0x08, 0x14, 0x17, 0x15, 0x1C, 0x17, 0x18, 0x15, 0x19, 0x17,
    0x16, 0x18, 0x17, 0x19, 0x1E, 0x1A, 0x1A, 0x16, 0x13, 0x14, 0x13, 0x14,
    0x13, 0x10, 0x14, 0x13, 0x08, 0x0D, 0x12, 0x08, 0x1A, 0x13, 0x15, 0x14,
    0x13, 0x0E, 0x12, 0x0F, 0x13, 0x14, 0x1C, 0x16, 0x14, 0x11, 0x10, 0x13,
    0x08, 0x08, 0x0A, 0x0A, 0x14, 0x0A, 0x0A, 0x09, 0x08, 0x08, 0x0D, 0x0D,
    0x0C, 0x0C, 0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x10, 0x10, 0x11, 0x11,
    0x0F, 0x0F, 0x0F, 0x0F, 0x0C, 0x0C, 0x19, 0x19, 0x0C, 0x0C, 0x0C, 0x1E,
    0x1E, 0x1C, 0x16, 0x1E, 0x1B, 0x1A, 0x18, 0x1C, 0x1C, 0x1B, 0x1C, 0x1C,
    0x1A, 0x1C, 0x1A, 0x18, 0x1A, 0x1A, 0x1A, 0x19, 0x1B, 0x1C, 0x18, 0x1A,
    0x1A, 0x1C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
];

for (var i = 0; i < 64 * 39; i++) {
    if (i < westernWidths.length) {
        root.Fonts["Default"].Widths[i] = westernWidths[i];
    } else {
        root.Fonts["Default"].Widths[i] = root.Fonts["Default"].DesignColWidth;
    }
}