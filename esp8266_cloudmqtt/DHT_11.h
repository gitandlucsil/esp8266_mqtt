#include "General.h"
#include "DHT.h"

#define DHTPIN_1 D4
#define DHTPIN_2 D3
#define DHTPIN_3 D2
#define DHTTYPE DHT11

void inicializaDHT11(void);
void lerDHT11(int *umi, int *tem, int id);
