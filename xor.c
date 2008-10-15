#include <stdint.h>
#include <stdio.h>

struct dnsRecords
{
  uint16_t rName;
  uint16_t rType;
  uint16_t rClass;
  uint32_t rTtl;
  uint16_t rdLength;
  uint32_t rRDATA;
};

int main()
{

  printf("uint32_t mide %i bytes\n", sizeof(uint32_t));
  printf("uint16_t mide %i bytes\n", sizeof(uint16_t));
  printf("El registro es de %i bytes\n", sizeof(struct dnsRecords));

}
