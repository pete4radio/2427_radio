/*
	This file is part of SX128x Linux driver.
	Copyright (C) 2020 ReimuNotMoe

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "SX128x_Linux.hpp"
#include "pins.hpp"

#include <cstdio>
#include <cstring>

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
	if (argc != 1)
	{
		printf("Usage: Chip_ID\n");
		return 1;
	}

// Pins assignment
	SX128x_Linux Radio("/dev/spidev0.0", 0, Hunter_pins);

	// Assume we're running on a high-end Raspberry Pi,
	// so we set the SPI clock speed to the maximum value supported by the chip
	Radio.SetSpiSpeed(8000000);
    std::cout << "Firmware Version: " << Radio.GetFirmwareVersion() << "\n";
	if (Radio.GetFirmwareVersion() != 43447) {std::cout << "Unexpected Chip ID.";}
	exit(EXIT_SUCCESS);
}