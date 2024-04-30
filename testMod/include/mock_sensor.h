#include <pgmspace.h>
#define mock_gz_len 1644
const uint8_t mock_html_gz[] PROGMEM = {
0x1F,0x8B,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x95,0x99,0xCB,0x6E,0x24,0xC9,
0x0D,0x45,0xF7,0xFE,0x8C,0x5A,0x37,0x12,0x41,0x32,0xF8,0x88,0xFE,0x15,0xC3,0x8B,
0x1E,0x5B,0xF0,0x34,0xE0,0x9E,0xB1,0xFB,0xB1,0x30,0x0C,0xFF,0xBB,0x6F,0x54,0x4F,
0xA6,0x8A,0x8C,0x32,0xB2,0x72,0xA9,0x92,0xA0,0xA3,0x08,0x92,0x87,0x64,0xE8,0xCF,
0xFF,0xB9,0x7D,0x7B,0xFB,0xED,0xDB,0xEF,0x5F,0x6F,0x1F,0x6F,0xBF,0x7C,0xFA,0xF6,
0x76,0xFB,0x70,0xFB,0xFE,0xF9,0xCB,0xDB,0xED,0x23,0x39,0x49,0xF7,0x61,0xCA,0xF8,
0xE4,0xED,0xCB,0x3F,0xDF,0xBE,0x7E,0xFA,0xFE,0xE3,0x2B,0xBE,0xC1,0xBC,0xA9,0xC5,
0x50,0x56,0xFB,0x70,0xFB,0xF5,0xC7,0x97,0xCF,0x7F,0xFB,0xFC,0xFD,0xDF,0xB7,0x8F,
0x1A,0x5B,0x6B,0xE2,0xEA,0x3D,0x3E,0xDC,0xFE,0xF5,0xE3,0xD3,0x3F,0xEE,0x9F,0x52,
0x6B,0xBD,0xE1,0xE7,0xFE,0xFA,0x3B,0xDF,0x3E,0x0A,0x7E,0xD5,0x2F,0x5F,0x3F,0xFF,
0xFD,0xD7,0xEF,0xBF,0xBD,0x7D,0xFB,0x86,0xAF,0xED,0xBF,0x1F,0xCE,0xF0,0xF6,0x0C,
0x1F,0x2A,0x4C,0x5D,0x12,0xDE,0xB7,0x21,0x5D,0x55,0x9B,0x26,0x3C,0x49,0xF0,0x8E,
0x97,0x8C,0xE7,0x38,0xC7,0xFB,0x13,0xBC,0xB5,0xE6,0xDD,0x48,0x2B,0x9E,0xC5,0x59,
0x49,0x12,0x9E,0x6D,0x1C,0xF8,0x5E,0xF0,0xFD,0x1C,0x1F,0xCF,0xF0,0x84,0xF3,0x1B,
0x79,0xC5,0x37,0x13,0xC4,0x85,0x12,0x5E,0x34,0xFA,0x8E,0xD7,0x82,0xA7,0x73,0xFC,
0x78,0x86,0xE7,0xD1,0xDC,0x07,0x15,0x7C,0x44,0x38,0x8F,0x4C,0xD7,0x16,0xBA,0xD3,
0xAD,0x84,0xBE,0x9D,0xD2,0xBD,0x3D,0xA3,0x77,0x09,0x47,0xF0,0x17,0x7A,0x97,0xE6,
0x61,0x09,0x6F,0xCC,0xC7,0xDD,0x7B,0xC1,0xAB,0x9E,0xF3,0xE9,0x19,0xDF,0x98,0xA4,
0x89,0x55,0x3E,0x69,0xA7,0xDE,0x73,0xEC,0x5D,0xDF,0xF9,0x51,0xF8,0xFE,0x02,0x9F,
0x9F,0xF1,0xA3,0xE1,0x02,0x6A,0xEA,0x21,0x1E,0x0D,0x3C,0x4F,0xF8,0xD1,0x47,0xEC,
0xF8,0x91,0xF1,0xF4,0x02,0x5D,0x9E,0xD0,0xBD,0x91,0x0F,0xB1,0x9A,0x7A,0xD1,0xA8,
0x87,0x8D,0x8C,0x0F,0x75,0xFA,0x03,0xDF,0x5B,0x49,0xBD,0x71,0x8E,0xEF,0x0B,0x5E,
0xB6,0x60,0x63,0xE9,0xB3,0x26,0xDE,0xF1,0x0E,0xED,0xB0,0x99,0x8F,0x31,0x1E,0xF0,
0xD1,0xCC,0xF6,0xC3,0x77,0x2A,0x77,0x7F,0x9E,0xF8,0xBE,0x4A,0xAF,0xC3,0x3A,0x22,
0xCC,0x9A,0xAC,0x13,0xB6,0xB5,0xD1,0x38,0x8C,0x1F,0xAD,0xE3,0xF8,0x2B,0xF7,0xC4,
0xEF,0x7C,0xB9,0xEC,0x7C,0x75,0x9E,0xE2,0x90,0x4E,0xC8,0xFC,0x14,0xF9,0x88,0xCD,
0x63,0xB8,0x73,0x7B,0x4C,0x3C,0x1B,0x4A,0x7B,0xDE,0xF5,0xAA,0xBC,0x17,0xE8,0xAB,
0xF2,0x74,0x13,0xF7,0xF0,0x2E,0x29,0xF0,0xA3,0x6D,0x9D,0x2C,0x84,0x52,0xE0,0xCD,
0x9B,0xEE,0xCA,0xE9,0xC5,0x78,0xC3,0xCF,0xE9,0xAB,0xF1,0x74,0xEB,0x32,0xF4,0x1E,
0x92,0x04,0xBF,0x5F,0x14,0xDB,0x63,0xB7,0xB1,0x99,0x08,0x3B,0xBC,0xFA,0xEE,0x05,
0xF8,0xEA,0x3B,0x1C,0xBD,0x5B,0x0B,0xB8,0xA5,0xD0,0x47,0x80,0x4D,0x66,0xE9,0xE8,
0x5D,0x8E,0xA3,0xDB,0xE5,0x5E,0x13,0xAB,0xEF,0x74,0xA3,0x88,0xD6,0x87,0xD4,0xB3,
0xA3,0xCF,0xA8,0x88,0x3E,0xEA,0xD6,0xC2,0x67,0x61,0xFE,0xA4,0x57,0xDD,0x9D,0xB7,
0x9A,0x58,0x6D,0x87,0xA4,0x83,0x55,0xD0,0x6C,0xA2,0x24,0x5D,0x9B,0x61,0x6F,0x33,
0xBA,0xEF,0x29,0x4F,0xE4,0x47,0xD2,0x15,0xD9,0xF1,0xB9,0x6D,0x62,0x75,0x5D,0x87,
0x54,0x0D,0xF0,0x99,0x4D,0x0F,0xF4,0xBE,0x75,0x94,0xBA,0x51,0xA4,0x82,0xEB,0x9D,
0xDA,0x4E,0x1F,0x97,0xE3,0x1E,0xAB,0xEB,0xFA,0x66,0x4A,0x26,0x2C,0xA9,0xD3,0xC4,
0x4C,0xF9,0x1E,0x3D,0xE4,0xB1,0xE0,0x90,0xB3,0x7D,0x97,0x8D,0x56,0xD5,0xBD,0x40,
0x5F,0x55,0x07,0xD9,0xC0,0xE6,0x3F,0x2B,0xE9,0x41,0x75,0xB6,0x09,0x77,0x1E,0xDA,
0x1E,0x6F,0x3E,0xB8,0x8D,0x3D,0xEE,0x4A,0xD7,0xB3,0xEE,0x99,0xEA,0x64,0x10,0x8F,
0xDE,0x53,0xDC,0x5D,0xB6,0x39,0x4E,0x60,0xBE,0xE2,0x47,0xBA,0x9A,0xEC,0xE3,0x9D,
0xD6,0xF1,0xEE,0xBC,0xC7,0xC7,0xAA,0x3A,0xD0,0x31,0x47,0xD2,0xC8,0x5D,0xC6,0x09,
0x16,0xA0,0x68,0xD1,0x1F,0x65,0x13,0xA1,0x7D,0xAF,0x38,0xAD,0xAA,0x3B,0x6F,0x32,
0xB1,0xAA,0xAE,0x6F,0xCC,0x34,0x44,0x39,0xD1,0x6D,0x6C,0x22,0x18,0x70,0xEE,0xBD,
0xE7,0xA0,0x0F,0xE4,0xE7,0x71,0xF3,0x75,0xB8,0x7B,0x21,0xEE,0xAB,0xEA,0xFA,0x46,
0xDC,0x9B,0xF0,0xC8,0x74,0x14,0xA2,0x35,0x15,0xB2,0x44,0xEF,0xE4,0xB2,0xD3,0x8B,
0xEB,0xE8,0x85,0xB3,0xAF,0xAE,0x93,0x0D,0x43,0xBD,0x87,0xE4,0x7A,0xD7,0x01,0xDB,
0x30,0xAA,0x3D,0x99,0x76,0xA0,0x43,0xEF,0x2D,0x4E,0x8B,0xEB,0xE8,0x3C,0xEE,0x63,
0x75,0x9D,0x60,0xBA,0x80,0x6B,0xBB,0xE6,0xD9,0xCE,0x36,0xB6,0x39,0x5C,0xD8,0xC8,
0xB3,0x1D,0xA4,0xB8,0xE3,0xFD,0x3A,0x7E,0x95,0x9D,0x6C,0x58,0x14,0xE6,0xFA,0x92,
0x54,0xAB,0x98,0x79,0x04,0x05,0xCA,0x92,0xF0,0xC4,0xE6,0x47,0xE4,0x8B,0xED,0xE8,
0xBC,0xE6,0xC6,0xB3,0xC9,0x6E,0xC0,0xF2,0xE3,0xAE,0xB5,0x84,0x47,0x2A,0x72,0x69,
0xF0,0x50,0x2D,0xB5,0xE3,0xF2,0xEB,0x68,0x77,0x1E,0xFA,0xF1,0x6C,0xB4,0x33,0xEA,
0x63,0x6E,0x4B,0x05,0x3F,0x48,0xC7,0x7D,0x87,0x78,0xA7,0x33,0x61,0xF9,0xFB,0x83,
0x5E,0x6C,0x47,0x7C,0x0E,0x9F,0x69,0x53,0xE1,0x8C,0xDD,0x85,0x7A,0x1B,0xF5,0xEA,
0x11,0x28,0xA7,0x9E,0x16,0x3A,0x46,0x1D,0xEE,0xC2,0xA1,0xEB,0xF4,0xD5,0x76,0xB4,
0xCD,0x85,0x02,0x8B,0x53,0x3D,0x3A,0x56,0x37,0xB4,0xE4,0x96,0x02,0x2F,0x24,0x87,
0x6C,0xF9,0x3A,0x7D,0xB5,0x1D,0xC1,0xF4,0x32,0x75,0x9B,0x67,0x6A,0xFC,0x51,0x18,
0x9F,0x95,0x72,0xDC,0xC5,0x7A,0xDF,0xE9,0x72,0x9D,0xBE,0xDA,0x8E,0xE6,0xF4,0xEA,
0x68,0x35,0x51,0xE8,0x02,0xB8,0xDF,0x9B,0xF2,0x03,0x1D,0xC6,0x39,0x26,0xFA,0x42,
0x97,0x73,0xFA,0x6A,0xBB,0xFB,0x08,0x27,0xEC,0x2D,0x75,0x39,0x45,0xF3,0x73,0xEC,
0xF1,0xEE,0x69,0x99,0x94,0xC0,0xDA,0xBD,0xE7,0xFC,0xF5,0xB3,0xAF,0xB6,0x6B,0x9B,
0xC2,0x35,0xE8,0x67,0x39,0xEB,0x74,0x53,0xEB,0x73,0xCF,0x48,0x71,0xC7,0x78,0x61,
0x3B,0xBD,0xDA,0xEE,0x8C,0x1E,0xAD,0xAD,0xB6,0x6B,0x68,0x28,0x03,0xDF,0xF0,0x22,
0xBB,0x01,0x05,0xA1,0xE6,0x34,0xC3,0xC7,0x71,0xF4,0xEA,0xBA,0xB3,0x8B,0x07,0x7C,
0x75,0x5D,0xDB,0x30,0xC4,0x28,0x0A,0xBB,0x2E,0x72,0x50,0xE0,0xE8,0x39,0xEA,0xB8,
0xA3,0xB6,0x0F,0x37,0xD5,0x74,0x2F,0xC0,0x17,0xD3,0xD1,0xD8,0xE6,0x90,0x8F,0x60,
0xF6,0x02,0x27,0xC2,0xF8,0x4C,0x9E,0x5C,0xD3,0x9D,0x8F,0x27,0x84,0x2A,0xBA,0x17,
0xEE,0x7D,0x11,0x1D,0xE8,0x21,0xC6,0xB3,0x77,0x16,0x3A,0xC6,0x2A,0xF4,0x53,0xCB,
0x17,0xEF,0x58,0x70,0x76,0xD7,0x5C,0x55,0x1D,0xF0,0xCB,0x60,0x07,0xBC,0x93,0xC1,
0x6A,0x9D,0x6B,0x8B,0x95,0xA9,0x55,0x4A,0x4F,0x67,0xDD,0xDE,0x27,0x7A,0xAA,0xAE,
0x3B,0xD3,0x3C,0xF0,0x8B,0xEB,0x80,0x57,0x0C,0xD5,0x12,0xD2,0x0B,0x1E,0x0D,0xBE,
0xCD,0x3A,0x79,0xC4,0x23,0xF0,0xB4,0x47,0x9E,0xAE,0xCA,0x0E,0xF8,0x45,0x76,0xC0,
0x0B,0xF2,0x9B,0xEA,0xBB,0xDD,0x5C,0xDA,0x8C,0x28,0xA5,0x9D,0x22,0x1E,0xC7,0xD5,
0x5F,0x75,0x1D,0xE0,0x8B,0xEB,0x00,0xE7,0xF9,0x18,0x28,0x9A,0xEB,0x1D,0xDB,0x8C,
0x05,0xF4,0x60,0xC9,0x36,0x0A,0xD9,0xED,0xF3,0x05,0x55,0xD9,0xBD,0x80,0x5F,0x64,
0x07,0x7C,0x9B,0x2F,0x52,0x5E,0xDE,0x6E,0xC6,0x46,0x86,0xD5,0xBE,0xE5,0xF1,0x46,
0xD1,0x78,0xF6,0xB4,0xA7,0x6A,0xBB,0x17,0xAA,0x6E,0xB1,0x1D,0xC5,0x36,0x3F,0x0E,
0x2D,0x91,0x8F,0x0D,0x1A,0x20,0x6C,0x98,0xE9,0xF2,0x8D,0x9A,0x1F,0x97,0x7F,0x5D,
0x77,0xB4,0xE8,0x0E,0x78,0xC5,0x36,0x63,0xD1,0xA2,0xE0,0x11,0x11,0x8C,0x5C,0xF9,
0xF4,0x86,0x1D,0x63,0xDF,0xE2,0xA9,0x0A,0xEF,0x05,0xFC,0x22,0x3C,0xE0,0x31,0x59,
0x92,0x30,0x57,0x3C,0xEC,0x3F,0x27,0x92,0x14,0x7B,0x43,0xDF,0xDB,0x75,0x4B,0x55,
0x79,0xE7,0x65,0x47,0xAB,0xF2,0x70,0x4C,0x22,0x41,0xAB,0xD5,0x8A,0xE7,0x59,0x74,
0x3D,0x3D,0x5B,0xCE,0x61,0xEF,0xB8,0xFC,0xEA,0xBC,0xF3,0xD8,0xD3,0xEA,0xBC,0xF9,
0x3C,0xEA,0xB0,0x4B,0x78,0xC1,0xDF,0x53,0x85,0xF2,0x84,0x83,0x86,0x7C,0xCC,0x57,
0x5C,0x9D,0xF7,0x02,0x7E,0x75,0xDE,0x2C,0x70,0x0B,0x4C,0x97,0x55,0x3A,0x0C,0xC1,
0xF4,0xC6,0x69,0xC4,0x71,0x63,0xDA,0x37,0x3A,0xAE,0xCE,0x7B,0x01,0xBF,0x3A,0x0F,
0x5D,0x0D,0x11,0x36,0xEF,0xA3,0x16,0x5E,0xC7,0x4E,0x25,0x79,0xBA,0x84,0x88,0x78,
0x77,0x1E,0x5F,0x77,0x1E,0xAD,0xCE,0x43,0x5F,0x73,0xB4,0xD4,0x28,0x8F,0xA6,0x53,
0x07,0x18,0x26,0x3D,0x2D,0x94,0xD8,0x6F,0xFB,0x31,0xD8,0x73,0xB5,0xDE,0x0B,0xA9,
0xB7,0x5A,0xCF,0xC1,0x19,0x0D,0x9D,0x24,0x65,0xBE,0xD1,0xE6,0xF3,0x65,0x45,0x45,
0xFF,0xEF,0xE5,0x5F,0xB7,0x1E,0xAD,0xD6,0xB3,0x0D,0xAB,0x1B,0x53,0x7E,0x45,0x31,
0xDE,0x14,0x3B,0xFE,0x68,0x52,0x42,0xEF,0x7E,0xD0,0xAF,0x2E,0xB4,0xA0,0xAF,0xD2,
0x33,0x74,0x5B,0x42,0x67,0xF3,0x8C,0xC7,0xAA,0xA7,0x6A,0xC3,0x2D,0xE3,0xE3,0xFE,
0xD0,0xF5,0x13,0x7F,0x5D,0x7A,0xBC,0x4A,0xCF,0xE6,0x31,0x15,0xA7,0x1F,0xE5,0xF4,
0xD8,0x61,0x08,0x19,0x91,0xCA,0x1E,0x93,0xD7,0xE1,0x5C,0xBE,0x2E,0x3D,0x5E,0xA5,
0x67,0x48,0xFC,0x39,0x84,0xE4,0x17,0xEB,0x79,0xFA,0xD1,0x86,0xB4,0x3C,0x6B,0xA4,
0xC4,0xBF,0xBA,0xD1,0x02,0xBF,0x4A,0x0F,0x8B,0xBB,0xDE,0x9F,0x4C,0x46,0xC1,0xFB,
0x7C,0xD6,0x93,0xF4,0x60,0x3E,0xC7,0xF0,0xF7,0xC4,0xBF,0x3E,0xE8,0xF1,0x2A,0x3D,
0x43,0x63,0x1D,0x64,0x96,0xFF,0x49,0x69,0x7D,0x43,0x63,0xC7,0xFA,0x9C,0x53,0x2F,
0x58,0x8F,0xCB,0x97,0xEB,0x83,0x1E,0xAF,0xD2,0x03,0x9E,0xD4,0xB0,0x43,0xE5,0xD4,
0x9B,0xAF,0x19,0xD8,0x20,0x3D,0xFF,0x93,0x32,0xDA,0xFB,0x6E,0x23,0xEB,0x52,0xFB,
0x97,0x3F,0xFD,0x0F,0x8D,0x95,0x65,0x8E,0xA5,0x1D,0x00,0x00};