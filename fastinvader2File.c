#include "sounds.h"
int fastinvader2_sample_rate = 11025;
int fastinvader2_num_samples = 1042;
int fastinvader2_array[] = {

128,128,116,94,71,54,31,20,9,20,
31,48,65,88,105,128,139,161,178,190,
207,218,229,240,252,252,252,255,255,252,
252,252,240,240,229,224,212,207,201,184,
184,173,161,150,144,128,128,122,111,99,
94,82,77,65,60,54,54,71,71,82,
88,94,94,94,82,77,65,60,37,31,
15,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,3,9,20,26,43,54,65,71,88,
99,116,122,133,144,156,167,178,184,195,
207,218,229,235,246,252,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,252,
246,240,235,224,218,218,207,201,195,190,
184,178,173,167,161,156,144,139,144,133,
128,128,122,116,116,105,105,94,94,88,
82,77,77,65,65,60,60,54,54,48,
43,43,37,37,31,31,26,26,15,3,
0,0,0,0,0,0,0,0,0,0,
0,0,0,20,37,54,71,82,94,105,
111,122,128,133,133,139,133,139,139,139,
133,139,128,128,128,116,116,111,111,99,
88,94,82,77,71,65,60,60,48,43,
37,31,37,43,54,71,77,88,94,94,
88,88,77,71,60,54,43,31,20,15,
3,0,0,0,0,0,0,0,0,0,
0,3,3,15,20,31,43,48,60,71,
82,99,111,122,128,139,150,167,178,190,
201,212,224,235,246,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,252,240,235,224,218,218,207,
201,195,190,178,178,167,156,150,144,139,
133,128,122,116,116,105,99,94,94,82,
71,77,71,60,54,54,48,43,37,31,
26,26,26,20,15,3,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
15,31,48,60,77,88,94,99,105,116,
116,122,122,128,122,122,116,116,111,105,
99,94,88,82,77,65,60,54,48,48,
37,31,26,20,15,9,3,0,3,0,
9,26,31,43,48,54,48,48,43,31,
26,15,3,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,15,20,31,43,54,65,82,
94,105,122,128,144,156,167,184,190,207,
218,229,240,252,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,252,
240,235,229,224,218,207,201,195,190,184,
178,178,167,161,156,144,139,133,128,133,
128,116,111,111,105,94,88,77,71,65,
60,60,54,54,48,48,43,43,37,31,
26,20,3,0,0,0,0,0,0,0,
0,0,0,0,0,15,26,48,65,82,
94,111,122,128,128,133,133,139,139,144,
139,144,139,139,133,128,128,116,116,111,
99,94,88,82,82,71,60,54,48,43,
37,31,26,20,26,20,26,43,48,60,
65,65,65,71,65,54,48,43,31,20,
9,3,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,9,20,
26,43,48,60,77,88,105,116,128,133,
144,161,173,190,201,212,218,229,240,252,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,
255,255,255,252,246,240,235,224,218,212,
201,195,184,184,178,167,161,156,150,144,
139,128,128,128,116,116,111,99,99,94,
88,82,82,77,77,65,60,54,54,48,
43,43,37,31,31,26,20,26,20,15,
15,20,15,15,15,15,9,9,9,9,
15,9,9,15,9,3,9,15,15,9,
15,9,15,15,20,15,20,26,26,20,
31,31,31,37,31,37,37,43,43,48,
54,48,54,54,60,60,65,65,71,71,
77,77,82,82,88,88,94,99,94,99,
105,105,111,116,111,116,116,122,122,128,
128,128,133,128,133,139,133,139,144,139,
144,144,150,156,156,156,150,156,161,156,
161,156,161,161,167,167,173,167,167,173,
167,173,173,167,167,173,173,178,173,178,
173,178,178,173,178,178,173,178,173,178,
178,173,173,178,173,178,184,178,184,178,
173,178,178,173,178,173,178,178,173,173,
178,178,173,167,173,178,173,167,167,173,
167,173,167,161,167,161,161,156,150,150,
156,150,144,144,150,144,144,150,144,139,
139,144,139,144,139,144,139,133,133,133,
139,133,139,133,128,133,133,128,128,133,
128,133,128,133,128,133,128,128,128,128,
128,128,128,128,128,128,128,128,128,122,
128,122,128,122,122,122,116,122,116,116,
122,122,116,122,116,111,116,116,111,111,
116,111,116,111,116,111,111,111,116,111,
116,111,116,111,111,116,111,111,111,111,
111,116};
