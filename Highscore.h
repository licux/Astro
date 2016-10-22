#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

#include "Arduboy.h"
#include "EEPROM.h"

class Highscore {
public:
	static const int16_t MEMORY_MAX = 5;

private:
	int16_t score[MEMORY_MAX];

public:
	Highscore();
	~Highscore();

	int16_t readScore(int16_t rank);
	void writeScore(int16_t _score);
};

Highscore::Highscore()
{
	for (int i = 0; i < MEMORY_MAX; i++) {
		uint8_t hi = EEPROM.read(2 * i);
		uint8_t lo = EEPROM.read(2 * i + 1);

		if ((hi == 0xFF) && (lo == 0xFF))
		{
			score[i] = 0;
		}
		else
		{
			score[i] = (hi << 8) | lo;
		}
	}
}

Highscore::~Highscore()
{

}

int16_t Highscore::readScore(int16_t rank)
{
	return score[rank];
}

void Highscore::writeScore(int16_t _score)
{
	for (int i = 0; i < MEMORY_MAX; i++) {
		if (score[i] < _score) {
			for (int j = MEMORY_MAX - 1; j > i; j--) {
				score[j] = score[j - 1];
				EEPROM.write(2 * j, (score[j] >> 8) & 0xFF);
				EEPROM.write(2 * j + 1, score[j] & 0xFF);
			}
			score[i] = _score;
			EEPROM.write(2 * i, (score[i] >> 8) & 0xFF);
			EEPROM.write(2 * i + 1, score[i] & 0xFF);
			break;
		}
	}
}

#endif __HIGHSCORE_H__