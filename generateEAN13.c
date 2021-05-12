#define MODULE	7
#define DIGITS	12
#define WIDTH 113

enum parity_e {
    PARITY_ODD = 0,
    PARITY_EVEN = 1
};

int checksum=0;
char toEncode[15] = {0};
int lenOfToEncode = 0;
bool data[113];
char chardata[113];

bool ticho[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
bool bok[] = {1, 0, 1};
bool strednik[] = {0, 1, 0, 1, 0};

int sticho = sizeof(ticho);
int sbok = sizeof(bok);
int sstrednik = sizeof(strednik);

static bool modulLN[10][7] = {
    {0, 0, 0, 1, 1, 0, 1}, {0, 0, 1, 1, 0, 0, 1}, {0, 0, 1, 0, 0, 1, 1}, {0, 1, 1, 1, 1, 0, 1},
    {0, 1, 0, 0, 0, 1, 1}, {0, 1, 1, 0, 0, 0, 1}, {0, 1, 0, 1, 1, 1, 1}, {0, 1, 1, 1, 0, 1, 1},
    {0, 1, 1, 0, 1, 1, 1}, {0, 0, 0, 1, 0, 1, 1}
};

static bool modulLP[10][7] = {
    {0, 1, 0, 0, 1, 1, 1}, {0, 1, 1, 0, 0, 1, 1}, {0, 0, 1, 1, 0, 1, 1}, {0, 1, 0, 0, 0, 0, 1},
    {0, 0, 1, 1, 1, 0, 1}, {0, 1, 1, 1, 0, 0, 1}, {0, 0, 0, 0, 1, 0, 1}, {0, 0, 1, 0, 0, 0, 1},
    {0, 0, 0, 1, 0, 0, 1}, {0, 0, 1, 0, 1, 1, 1}
};

static bool modulP[10][7] = {
    {1, 1, 1, 0, 0, 1, 0}, {1, 1, 0, 0, 1, 1, 0}, {1, 1, 0, 1, 1, 0, 0}, {1, 0, 0, 0, 0, 1, 0},
    {1, 0, 1, 1, 1, 0, 0}, {1, 0, 0, 1, 1, 1, 0}, {1, 0, 1, 0, 0, 0, 0}, {1, 0, 0, 0, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 0}, {1, 1, 1, 0, 1, 0, 0}
};

static bool parita[10][5] = {
    {0, 0, 0, 0, 0}, {0, 1, 0, 1, 1}, {0, 1, 1, 0, 1}, {0, 1, 1, 1, 0}, {1, 0, 0, 1, 1},
    {1, 1, 0, 0, 1}, {1, 1, 1, 0, 0}, {1, 0, 1, 0, 1}, {1, 0, 1, 1, 0}, {1, 1, 0, 1, 0}
};

int width() {
    return WIDTH;
};

static void toDataArr(bool *dest, bool *src, int n)
{
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void prettyprint(bool* arr)
{
    for (int i = 0; i < width(); i++) {
    	chardata[i]=0x30+arr[i];
    	if(i > 11 && ((i < 7*7+11 &&  (i-12) %7 == 0) || (i >= 7*6+17 &&  (i-17) %7 == 0) && i<(13*7+17)))
    	{
    		printf(" ");
    	}
        printf("%d", (int)arr[i]);
    }
    printf("\r\n");
}

int build(char *toEncode, bool *data)
{
	int num=0;
	int start=0;
 
	int index=0;

    for (int i = 0; i < WIDTH; i++) {
        data[i] = 0;
    }

    toDataArr(data, ticho, sticho);
    index += sticho;

    toDataArr(data + index, bok, sbok);
    index += sbok;

    start = toEncode[0] - 0x30;

    num = toEncode[1] - 0x30;
    toDataArr(data + index, modulLN[num], 7);
    index += 7;

    for (int i = 2; i < 7; i++) {
        num = toEncode[i] - 0x30;
        if (parita[start][i - 2] == 0) {
            toDataArr(data + index, modulLN[num], 7);
        } else {
            toDataArr(data + index, modulLP[num], 7);
        }
        index += 7;
    }

    toDataArr(data + index, strednik, sstrednik);
    index += sstrednik;

    for (int i = 7; i < 12; i++) {
        num = toEncode[i] - 0x30;
        toDataArr(data + index, modulP[num], 7);
        index += 7;
    }

    int odds = 0;
    int evens = 0;
    for (int i = 0; i < 12; i++) {
        if (i % 2 == 0) {
            evens += toEncode[i] - 0x30;
        } else {
            odds += toEncode[i] - 0x30;
        }
    }

    checksum = 10 - (((odds * 3) + evens) % 10);
    toDataArr(data + index, modulP[checksum], 7);
    index += 7;

    toDataArr(data + index, bok, sbok);
    index += sbok;

    toDataArr(data + index, ticho, sticho);
    index += sticho;

    return 0;
}
//#Ladomirjak René, 230491, BPC_MET/02