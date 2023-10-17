#include <cmath>
#include <default_screen.hpp>

uint8_t default_screen::screen_data_32x32[] = {
 35,  49,  27,  36,  50,  28,  38,  52,  29,  39,  54,  30,  41,  56,  31,  42, 
 57,  33,  44,  59,  34,  45,  60,  34,  46,  61,  35,  47,  62,  36,  47,  63, 
 36,  49,  65,  37,  50,  66,  38,  51,  68,  39,  53,  69,  40,  54,  70,  41, 
 55,  72,  42,  56,  73,  43,  57,  74,  44,  57,  74,  44,  58,  75,  45,  57, 
 74,  44,  57,  74,  45,  58,  75,  45,  58,  75,  45,  58,  75,  45,  58,  75, 
 45,  57,  74,  45,  57,  74,  45,  57,  73,  45,  56,  73,  44,  56,  73,  44, 
 35,  50,  27,  37,  51,  28,  38,  53,  29,  40,  55,  30,  42,  57,  32,  43, 
 58,  33,  44,  60,  34,  46,  61,  35,  47,  62,  35,  48,  63,  36,  48,  64, 
 37,  50,  66,  38,  51,  67,  39,  52,  69,  40,  54,  70,  41,  55,  72,  42, 
 56,  73,  43,  57,  74,  43,  58,  75,  44,  59,  76,  45,  59,  76,  45,  59, 
 76,  45,  59,  76,  45,  59,  77,  45,  59,  77,  45,  59,  77,  45,  59,  76, 
 45,  59,  76,  45,  59,  76,  45,  58,  75,  45,  58,  75,  45,  58,  75,  45, 
 36,  50,  27,  37,  52,  28,  39,  54,  29,  41,  56,  31,  42,  58,  32,  44, 
 59,  33,  45,  61,  34,  47,  62,  35,  48,  64,  36,  49,  65,  36,  49,  66, 
 37,  51,  67,  38,  52,  69,  39,  54,  70,  40,  55,  72,  41,  56,  74,  42, 
 58,  75,  43,  59,  76,  44,  59,  77,  44,  60,  78,  45,  61,  79,  45,  60, 
 78,  45,  60,  78,  45,  61,  79,  46,  61,  79,  46,  61,  79,  46,  61,  79, 
 46,  60,  78,  46,  60,  78,  46,  60,  78,  45,  60,  77,  45,  59,  77,  45, 
 37,  52,  28,  38,  53,  29,  40,  55,  30,  42,  57,  31,  43,  59,  32,  45, 
 61,  33,  46,  62,  34,  48,  64,  35,  49,  65,  36,  50,  66,  37,  51,  67, 
 37,  52,  69,  38,  54,  71,  39,  55,  72,  40,  56,  74,  41,  58,  75,  42, 
 59,  77,  43,  60,  78,  44,  61,  79,  45,  62,  80,  45,  62,  81,  46,  62, 
 80,  45,  62,  81,  46,  62,  81,  46,  63,  81,  46,  63,  81,  46,  62,  81, 
 46,  62,  81,  46,  62,  80,  46,  62,  80,  46,  61,  80,  46,  61,  79,  45, 
 38,  53,  28,  39,  55,  29,  41,  57,  30,  43,  58,  31,  45,  60,  32,  46, 
 62,  34,  48,  64,  35,  49,  65,  35,  50,  67,  36,  51,  68,  37,  52,  69, 
 38,  54,  71,  39,  55,  72,  40,  57,  74,  41,  58,  76,  42,  59,  78,  43, 
 61,  79,  44,  62,  80,  45,  63,  82,  45,  64,  83,  46,  64,  83,  46,  64, 
 82,  46,  64,  83,  46,  64,  83,  46,  65,  84,  47,  65,  84,  47,  64,  84, 
 47,  64,  83,  46,  64,  83,  46,  64,  83,  46,  64,  83,  46,  63,  82,  46, 
 39,  54,  28,  40,  56,  29,  42,  58,  30,  44,  60,  31,  46,  62,  33,  47, 
 64,  34,  49,  66,  35,  50,  67,  36,  51,  68,  37,  52,  70,  37,  53,  71, 
 38,  55,  73,  39,  57,  74,  40,  58,  76,  41,  60,  78,  42,  61,  80,  43, 
 62,  81,  44,  64,  83,  45,  65,  84,  46,  66,  85,  46,  66,  86,  47,  66, 
 85,  46,  66,  86,  47,  66,  86,  47,  67,  86,  47,  67,  86,  47,  66,  86, 
 47,  66,  86,  47,  66,  86,  47,  66,  86,  46,  66,  85,  46,  65,  85,  46, 
 40,  56,  28,  42,  58,  29,  44,  60,  30,  45,  62,  32,  47,  64,  33,  49, 
 66,  34,  50,  68,  35,  52,  69,  36,  53,  70,  37,  54,  72,  38,  55,  73, 
 38,  57,  75,  39,  58,  77,  41,  60,  78,  42,  61,  80,  43,  63,  82,  44, 
 64,  84,  44,  66,  85,  45,  67,  87,  46,  68,  88,  47,  68,  88,  47,  68, 
 88,  47,  68,  88,  47,  68,  89,  47,  69,  89,  47,  69,  89,  47,  69,  89, 
 47,  69,  89,  47,  68,  89,  47,  68,  89,  47,  68,  88,  47,  68,  88,  46, 
 42,  58,  28,  43,  60,  29,  45,  62,  31,  47,  64,  32,  49,  66,  33,  51, 
 68,  34,  52,  70,  35,  53,  71,  36,  55,  73,  37,  56,  74,  38,  57,  75, 
 39,  58,  77,  40,  60,  79,  41,  62,  81,  42,  63,  83,  43,  65,  85,  44, 
 66,  86,  45,  67,  88,  46,  69,  89,  46,  70,  90,  47,  70,  91,  47,  70, 
 91,  47,  70,  91,  47,  71,  92,  48,  71,  92,  48,  71,  92,  48,  71,  92, 
 48,  71,  92,  47,  71,  92,  47,  70,  92,  47,  70,  92,  47,  70,  91,  47, 
 44,  61,  28,  45,  62,  30,  47,  64,  31,  49,  66,  32,  51,  68,  33,  52, 
 70,  35,  54,  72,  36,  55,  73,  37,  56,  75,  37,  57,  76,  38,  58,  77, 
 39,  60,  79,  40,  62,  81,  41,  63,  83,  42,  65,  85,  43,  67,  87,  44, 
 68,  89,  45,  69,  91,  46,  71,  92,  47,  72,  93,  47,  72,  94,  48,  72, 
 93,  47,  72,  94,  48,  73,  95,  48,  73,  95,  48,  73,  95,  48,  73,  95, 
 48,  73,  95,  48,  73,  95,  48,  73,  95,  47,  73,  95,  47,  73,  95,  47, 
 46,  63,  29,  47,  65,  30,  49,  67,  31,  51,  69,  32,  52,  71,  34,  54, 
 73,  35,  56,  74,  36,  57,  76,  37,  58,  77,  38,  59,  79,  38,  60,  80, 
 39,  62,  82,  40,  64,  84,  41,  65,  86,  42,  67,  88,  44,  69,  90,  45, 
 70,  92,  45,  72,  93,  46,  73,  95,  47,  74,  96,  48,  75,  97,  48,  74, 
 96,  47,  75,  97,  48,  75,  98,  48,  75,  98,  48,  75,  98,  48,  75,  98, 
 48,  75,  98,  48,  75,  98,  48,  75,  98,  48,  75,  98,  47,  75,  98,  47, 
 48,  66,  29,  49,  68,  30,  51,  69,  31,  53,  71,  33,  54,  73,  34,  56, 
 75,  35,  58,  77,  36,  59,  78,  37,  60,  80,  38,  61,  81,  39,  62,  82, 
 39,  64,  84,  41,  66,  86,  42,  67,  89,  43,  69,  91,  44,  71,  93,  45, 
 72,  94,  46,  74,  96,  47,  75,  98,  47,  76,  99,  48,  77, 100,  48,  76, 
 99,  48,  77, 100,  48,  77, 101,  48,  78, 101,  48,  78, 101,  48,  78, 101, 
 48,  78, 101,  48,  78, 101,  48,  78, 101,  48,  78, 101,  48,  78, 101,  47, 
 51,  70,  29,  52,  71,  30,  54,  73,  32,  56,  75,  33,  57,  77,  34,  59, 
 79,  35,  60,  81,  36,  62,  82,  37,  63,  84,  38,  64,  85,  39,  65,  86, 
 40,  67,  88,  41,  69,  90,  42,  70,  92,  43,  72,  95,  44,  74,  97,  45, 
 75,  99,  46,  77, 100,  47,  78, 102,  48,  79, 103,  48,  80, 104,  49,  80, 
104,  48,  80, 105,  48,  81, 105,  49,  81, 106,  49,  81, 106,  49,  81, 106, 
 49,  81, 106,  49,  81, 106,  48,  81, 106,  48,  81, 106,  48,  81, 106,  48, 
 54,  74,  29,  56,  76,  31,  57,  77,  32,  59,  79,  33,  60,  81,  35,  62, 
 83,  36,  64,  85,  37,  65,  86,  38,  66,  88,  39,  67,  89,  39,  68,  90, 
 40,  70,  92,  41,  72,  94,  43,  73,  97,  44,  75,  99,  45,  77, 101,  46, 
 79, 103,  47,  80, 105,  48,  82, 106,  48,  83, 108,  49,  84, 109,  49,  83, 
109,  49,  84, 109,  49,  84, 110,  49,  85, 110,  49,  85, 111,  49,  85, 111, 
 49,  85, 111,  49,  85, 111,  49,  85, 111,  48,  85, 111,  48,  85, 111,  48, 
 58,  78,  30,  59,  80,  31,  61,  82,  32,  62,  84,  34,  64,  86,  35,  65, 
 87,  36,  67,  89,  37,  68,  91,  38,  69,  92,  39,  70,  93,  40,  71,  95, 
 41,  73,  97,  42,  75,  99,  43,  77, 101,  44,  79, 103,  45,  80, 105,  46, 
 82, 108,  47,  84, 109,  48,  85, 111,  49,  86, 113,  49,  87, 114,  50,  87, 
113,  49,  88, 114,  49,  88, 115,  50,  88, 115,  50,  89, 116,  50,  89, 116, 
 50,  89, 116,  49,  89, 116,  49,  89, 117,  49,  89, 117,  49,  89, 117,  48, 
 61,  83,  30,  63,  85,  31,  64,  87,  33,  66,  88,  34,  67,  90,  35,  69, 
 92,  37,  70,  94,  38,  72,  95,  39,  73,  97,  40,  74,  98,  40,  75,  99, 
 41,  77, 101,  42,  79, 104,  43,  80, 106,  45,  82, 108,  46,  84, 110,  47, 
 86, 112,  48,  87, 114,  48,  89, 116,  49,  90, 118,  50,  91, 119,  50,  91, 
118,  49,  91, 119,  50,  92, 120,  50,  92, 121,  50,  93, 121,  50,  93, 121, 
 50,  93, 122,  50,  93, 122,  49,  93, 122,  49,  93, 122,  49,  94, 122,  49, 
 65,  88,  30,  66,  90,  32,  68,  91,  33,  70,  93,  34,  71,  95,  36,  73, 
 97,  37,  74,  98,  38,  75, 100,  39,  76, 101,  40,  78, 103,  41,  79, 104, 
 41,  80, 106,  43,  82, 108,  44,  84, 111,  45,  86, 113,  46,  88, 115,  47, 
 89, 117,  48,  91, 119,  49,  93, 121,  50,  94, 123,  50,  95, 124,  50,  95, 
124,  50,  95, 125,  50,  96, 125,  50,  96, 126,  50,  97, 126,  50,  97, 127, 
 50,  97, 127,  50,  97, 127,  50,  98, 128,  50,  98, 128,  49,  98, 128,  49, 
 69,  93,  31,  70,  95,  32,  72,  97,  33,  73,  98,  35,  75, 100,  36,  76, 
102,  37,  78, 103,  38,  79, 105,  40,  80, 106,  40,  81, 108,  41,  82, 109, 
 42,  84, 111,  43,  86, 113,  44,  88, 116,  46,  90, 118,  47,  92, 120,  48, 
 93, 122,  49,  95, 124,  49,  96, 126,  50,  98, 128,  51,  99, 129,  51,  99, 
129,  50,  99, 130,  51, 100, 131,  51, 100, 131,  51, 101, 132,  51, 101, 132, 
 51, 101, 133,  50, 101, 133,  50, 102, 133,  50, 102, 133,  50, 102, 134,  49, 
 73,  99,  31,  74, 100,  32,  76, 102,  34,  77, 103,  35,  79, 105,  36,  80, 
107,  38,  82, 108,  39,  83, 110,  40,  84, 111,  41,  85, 113,  42,  86, 114, 
 42,  88, 116,  44,  90, 118,  45,  92, 121,  46,  93, 123,  47,  95, 125,  48, 
 97, 127,  49,  99, 130,  50, 100, 131,  50, 102, 133,  51, 103, 135,  51, 103, 
134,  51, 103, 135,  51, 104, 136,  51, 104, 137,  51, 105, 137,  51, 105, 138, 
 51, 105, 138,  51, 106, 138,  50, 106, 139,  50, 106, 139,  50, 106, 139,  49, 
 77, 104,  31,  79, 105,  33,  80, 107,  34,  81, 109,  35,  83, 110,  37,  84, 
112,  38,  85, 114,  39,  87, 115,  40,  88, 116,  41,  89, 118,  42,  90, 119, 
 43,  92, 121,  44,  93, 123,  45,  95, 126,  46,  97, 128,  47,  99, 130,  48, 
101, 133,  49, 103, 135,  50, 104, 137,  51, 106, 138,  51, 107, 140,  51, 106, 
140,  51, 107, 140,  51, 108, 141,  51, 108, 142,  51, 109, 143,  51, 109, 143, 
 51, 109, 143,  51, 110, 144,  51, 110, 144,  50, 110, 145,  50, 110, 145,  50, 
 81, 109,  32,  83, 111,  33,  84, 112,  34,  85, 114,  36,  87, 116,  37,  88, 
117,  38,  89, 119,  40,  91, 120,  41,  92, 122,  41,  93, 123,  42,  94, 124, 
 43,  95, 126,  44,  97, 128,  45,  99, 131,  47, 101, 133,  48, 103, 135,  49, 
105, 138,  50, 106, 140,  50, 108, 142,  51, 109, 144,  51, 111, 145,  52, 110, 
145,  51, 111, 146,  51, 112, 146,  51, 112, 147,  51, 113, 148,  51, 113, 148, 
 51, 113, 149,  51, 114, 149,  51, 114, 150,  50, 114, 150,  50, 114, 150,  50, 
 85, 115,  32,  87, 116,  33,  88, 118,  35,  89, 119,  36,  91, 121,  37,  92, 
122,  39,  93, 124,  40,  94, 125,  41,  95, 127,  42,  96, 128,  42,  97, 129, 
 43,  99, 131,  44, 101, 133,  46, 103, 136,  47, 105, 138,  48, 107, 140,  49, 
108, 143,  50, 110, 145,  51, 112, 147,  51, 113, 149,  52, 114, 150,  52, 114, 
150,  51, 115, 151,  51, 115, 152,  51, 116, 152,  52, 116, 153,  51, 117, 153, 
 51, 117, 154,  51, 117, 154,  51, 118, 155,  50, 118, 155,  50, 118, 156,  50, 
 89, 120,  32,  91, 121,  34,  92, 123,  35,  93, 124,  36,  94, 126,  38,  96, 
127,  39,  97, 129,  40,  98, 130,  41,  99, 132,  42, 100, 133,  43, 101, 134, 
 43, 103, 136,  45, 105, 138,  46, 106, 141,  47, 108, 143,  48, 110, 145,  49, 
112, 147,  50, 114, 150,  51, 115, 152,  51, 117, 153,  52, 118, 155,  52, 118, 
155,  52, 119, 156,  53, 120, 157,  53, 120, 157,  53, 121, 158,  53, 121, 159, 
 53, 121, 159,  53, 122, 160,  53, 122, 160,  53, 123, 161,  52, 123, 161,  52, 
 91, 122,  32,  92, 124,  34,  94, 125,  35,  95, 127,  36,  96, 128,  38,  97, 
130,  39,  99, 131,  40, 100, 132,  41, 101, 134,  42, 102, 135,  43, 103, 136, 
 43, 104, 138,  45, 106, 141,  46, 108, 143,  47, 110, 145,  48, 112, 147,  49, 
114, 150,  50, 115, 152,  51, 117, 154,  51, 118, 156,  52, 120, 157,  52, 120, 
157,  53, 121, 158,  53, 121, 159,  53, 122, 160,  53, 122, 161,  53, 123, 161, 
 53, 123, 162,  53, 124, 162,  53, 124, 163,  53, 125, 163,  53, 125, 164,  52, 
 93, 125,  32,  94, 126,  34,  95, 128,  35,  97, 129,  37,  98, 130,  38,  99, 
132,  39, 100, 133,  40, 101, 135,  41, 103, 136,  42, 104, 137,  43, 105, 139, 
 44, 106, 141,  45, 108, 143,  46, 110, 145,  47, 112, 147,  48, 114, 150,  49, 
115, 152,  50, 117, 154,  51, 119, 156,  51, 120, 158,  52, 122, 160,  52, 122, 
160,  53, 123, 161,  53, 123, 162,  53, 124, 163,  53, 124, 163,  53, 125, 164, 
 53, 125, 164,  53, 126, 165,  53, 126, 166,  53, 127, 166,  53, 127, 167,  53, 
 95, 127,  33,  96, 129,  34,  97, 130,  35,  99, 131,  37, 100, 133,  38, 101, 
134,  39, 102, 136,  40, 103, 137,  41, 104, 138,  42, 105, 140,  43, 106, 141, 
 44, 108, 143,  45, 110, 145,  46, 112, 148,  47, 114, 150,  49, 115, 152,  50, 
117, 155,  50, 119, 157,  51, 121, 159,  52, 122, 161,  52, 123, 162,  52, 124, 
162,  53, 124, 163,  53, 125, 164,  53, 126, 165,  54, 126, 166,  54, 127, 166, 
 54, 127, 167,  53, 128, 168,  53, 128, 168,  53, 129, 169,  53, 129, 169,  53, 
 97, 130,  33,  98, 131,  34,  99, 133,  36, 100, 134,  37, 102, 135,  38, 103, 
137,  39, 104, 138,  41, 105, 140,  42, 106, 141,  42, 107, 142,  43, 108, 143, 
 44, 110, 145,  45, 112, 148,  46, 113, 150,  48, 115, 152,  49, 117, 155,  50, 
119, 157,  50, 121, 159,  51, 122, 161,  52, 124, 163,  52, 125, 165,  52, 126, 
165,  53, 126, 166,  53, 127, 167,  54, 128, 168,  54, 128, 168,  54, 129, 169, 
 54, 129, 170,  54, 130, 170,  53, 130, 171,  53, 131, 171,  53, 131, 172,  53, 
 99, 133,  33, 100, 134,  34, 101, 135,  36, 102, 137,  37, 104, 138,  38, 105, 
140,  40, 106, 141,  41, 107, 142,  42, 108, 144,  42, 109, 145,  43, 110, 146, 
 44, 112, 148,  45, 113, 150,  46, 115, 152,  48, 117, 155,  49, 119, 157,  50, 
121, 159,  51, 123, 162,  51, 124, 164,  52, 126, 166,  52, 127, 167,  52, 127, 
167,  53, 128, 168,  53, 129, 169,  54, 130, 170,  54, 130, 171,  54, 131, 172, 
 54, 131, 172,  54, 132, 173,  53, 132, 174,  53, 133, 174,  53, 133, 175,  53, 
101, 136,  33, 102, 137,  34, 103, 138,  36, 105, 139,  37, 106, 141,  39, 107, 
142,  40, 108, 144,  41, 109, 145,  42, 110, 146,  43, 111, 147,  43, 112, 149, 
 44, 114, 151,  45, 115, 153,  47, 117, 155,  48, 119, 157,  49, 121, 160,  50, 
123, 162,  51, 124, 164,  51, 126, 166,  52, 128, 168,  52, 129, 170,  52, 129, 
170,  53, 130, 171,  54, 131, 172,  54, 132, 173,  54, 132, 174,  54, 133, 174, 
 54, 133, 175,  54, 134, 176,  54, 134, 176,  53, 135, 177,  53, 135, 177,  53, 
103, 139,  33, 104, 140,  35, 105, 141,  36, 107, 142,  37, 108, 143,  39, 109, 
145,  40, 110, 146,  41, 111, 148,  42, 112, 149,  43, 113, 150,  43, 114, 151, 
 44, 116, 153,  45, 117, 155,  47, 119, 158,  48, 121, 160,  49, 123, 162,  50, 
125, 164,  51, 126, 167,  51, 128, 169,  52, 130, 171,  52, 131, 173,  52, 131, 
173,  53, 132, 174,  54, 133, 174,  54, 133, 175,  54, 134, 176,  54, 135, 177, 
 54, 135, 178,  54, 136, 178,  54, 136, 179,  54, 137, 179,  53, 137, 180,  53, 
106, 141,  33, 107, 142,  35, 108, 144,  36, 109, 145,  37, 110, 146,  39, 111, 
148,  40, 112, 149,  41, 113, 150,  42, 114, 151,  43, 115, 153,  43, 116, 154, 
 44, 118, 156,  45, 119, 158,  47, 121, 160,  48, 123, 162,  49, 125, 165,  50, 
126, 167,  51, 128, 169,  51, 130, 171,  52, 131, 173,  52, 133, 175,  52, 133, 
175,  53, 134, 176,  54, 135, 177,  54, 135, 178,  54, 136, 179,  54, 137, 179, 
 54, 137, 180,  54, 138, 181,  54, 138, 181,  54, 139, 182,  54, 139, 183,  53, 
108, 144,  33, 109, 145,  35, 110, 147,  36, 111, 148,  38, 112, 149,  39, 113, 
150,  40, 114, 152,  41, 115, 153,  42, 116, 154,  43, 117, 155,  43, 118, 156, 
 44, 119, 158,  45, 121, 160,  47, 123, 163,  48, 125, 165,  49, 127, 167,  50, 
128, 169,  51, 130, 172,  51, 132, 174,  52, 133, 176,  52, 135, 178,  52, 135, 
178,  53, 136, 179,  54, 137, 180,  54, 137, 180,  54, 138, 181,  54, 139, 182, 
 54, 139, 183,  54, 140, 183,  54, 140, 184,  54, 141, 185,  54, 141, 185,  54, 
110, 147,  34, 111, 148,  35, 112, 149,  36, 113, 151,  38, 114, 152,  39, 115, 
153,  40, 116, 154,  41, 117, 156,  42, 118, 157,  43, 119, 158,  43, 120, 159, 
 44, 121, 161,  45, 123, 163,  47, 125, 165,  48, 127, 167,  49, 128, 170,  50, 
130, 172,  50, 132, 174,  51, 134, 176,  52, 135, 178,  52, 137, 180,  52, 137, 
180,  54, 138, 181,  54, 139, 182,  54, 139, 183,  54, 140, 184,  54, 140, 185, 
 54, 141, 185,  54, 142, 186,  54, 142, 187,  54, 143, 187,  54, 143, 188,  54};

void default_screen::make(screen_buffer &sb)
{
    double xasp = 32.0 / sb.get_width();
    double yasp = 32.0 / sb.get_height();
    int xi, yi;
    pixel p;

    for (int x = 0; x < sb.get_width(); x++)
    {
        xi = /*round*/(xasp*x);

        for (int y = 0; y < sb.get_height(); y++)
        {
            yi = /*round*/(yasp*y);

            get_pixel(xi, yi, p);
            sb.put_pixel(x, y, p);
        }
    }
}

void default_screen::get_pixel(int x, int y, pixel &p)
{
    int offset = 32*3*y +3*x;
    p.r = screen_data_32x32[offset+0];
    p.g = screen_data_32x32[offset+1];
    p.b = screen_data_32x32[offset+2];
}
