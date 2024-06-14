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

#include <cstdio>
#include <cstring>
#include <fstream>

#include "pins.hpp"
#include "json.hpp"


int main(int argc, char **argv) {
	if ((argc != 2))
	{
		printf("Usage: LinkTestJ <rx|tx> \n"
	"typedef enum {\n"
	"	LORA_BW_0200 = 0x34,\n"
	"	LORA_BW_0400 = 0x26,\n"
	"	LORA_BW_0800 = 0x18,\n"
	"	LORA_BW_1600 = 0x0A,\n"
	"} RadioLoRaBandwidths_t;\n\n"


	"typedef enum {\n"
	"	LORA_SF5 = 0x50,\n"
	"	LORA_SF6 = 0x60,\n"
	"	LORA_SF7 = 0x70,\n"
	"	LORA_SF8 = 0x80,\n"
	"	LORA_SF9 = 0x90,\n"
	"	LORA_SF10 = 0xA0,\n"
	"	LORA_SF11 = 0xB0,\n"
	"	LORA_SF12 = 0xC0,\n"
	"} RadioLoRaSpreadingFactors_t;\n\n"


	"typedef enum {\n"
	"	LORA_CR_4_5 = 0x01,\n"
	"	LORA_CR_4_6 = 0x02,\n"
	"	LORA_CR_4_7 = 0x03,\n"
	"	LORA_CR_4_8 = 0x04,\n"
	"	LORA_CR_LI_4_5 = 0x05,\n"
	"	LORA_CR_LI_4_6 = 0x06,\n"
	"	LORA_CR_LI_4_7 = 0x07,\n"
	"} RadioLoRaCodingRates_t;\n");	

		return 1;
	};
// read the LoRa Parameters from the json file 
    std::cout << "before file read" << std::endl;
	std::ifstream ir("radio.json");
    std::cout << "after file read" << std::endl;
	nlohmann::json_abi_v3_11_3::json jr;
    std::cout << "before assignment" << std::endl;
	ir >> jr;
    std::cout << jr;
	std::cout << "after assignment" << std::endl;

	const char * mode = jr["Radio"][0]["Mode"].get<std::string>().c_str();
    std::cout << "Mode: [lora | flrc]" << mode << std::endl;

	int32_t freq = jr["Radio"][1]["Freq"];
    std::cout << "Frequency [2427 (MHz)]: " << freq << std::endl;

	int32_t power = jr["Radio"][2]["Power"];
    std::cout << "Transmit Power [-18 to 13 (dBm)]: " << power << std::endl;
	power = power + 18;
	if (power >= 31) power = 31;
	if (power <= 0) power = 0;

	int32_t BW = jr["Radio"][3]["BW"];
    std::cout << "Bandwidth: " << BW << std::endl;
	SX128x::RadioLoRaBandwidths_t BW_Reg;

	int32_t SF = jr["Radio"][4]["SF"];
    std::cout << "Spreading Factor: " << SF << std::endl;
	SX128x::RadioLoRaSpreadingFactors_t SF_Reg;

	int32_t CR = jr["Radio"][5]["CR"];
    std::cout << "Coding Rate: " << CR << std::endl;
	SX128x::RadioLoRaCodingRates_t CR_Reg;

	if (BW == 1600) BW_Reg = SX128x::LORA_BW_1600;
	if (BW == 800) BW_Reg = SX128x::LORA_BW_0800;
	if (BW == 400) BW_Reg = SX128x::LORA_BW_0400;
	if (BW == 200) BW_Reg = SX128x::LORA_BW_0200;

	if (SF == 5) SF_Reg = SX128x::LORA_SF5;
	if (SF == 6) SF_Reg = SX128x::LORA_SF6;
	if (SF == 7) SF_Reg = SX128x::LORA_SF7;
	if (SF == 8) SF_Reg = SX128x::LORA_SF8;
	if (SF == 9) SF_Reg = SX128x::LORA_SF9;
	if (SF == 10) SF_Reg = SX128x::LORA_SF10;
	if (SF == 11) SF_Reg = SX128x::LORA_SF11;
	if (SF == 12) SF_Reg = SX128x::LORA_SF12;

	if (CR == 5) CR_Reg = SX128x::LORA_CR_4_5;
	if (CR == 6) CR_Reg = SX128x::LORA_CR_4_6;
	if (CR == 7) CR_Reg = SX128x::LORA_CR_4_7;
	if (CR == 8) CR_Reg = SX128x::LORA_CR_4_8;

	int modmode = 0;

	if (strcmp(mode, "flrc") == 0) {
		modmode = 1;
	}

// Pins assignment
	std::cout << "Before Radio" << std::endl;

	SX128x_Linux Radio("/dev/spidev0.0", 0, Hunter_pins);

	std::cout << "after Radio" << std::endl;
	// Assume we're running on a high-end Raspberry Pi,
	// so we set the SPI clock speed to the maximum value supported by the chip
	Radio.SetSpiSpeed(8000000);
	std::cout << "after SPI speed set" << std::endl;
	Radio.Init();
	std::cout << "after Init" << std::endl;
	puts("Init done");
	Radio.SetStandby(SX128x::STDBY_XOSC);
	puts("SetStandby done");
	Radio.SetRegulatorMode(static_cast<SX128x::RadioRegulatorModes_t>(0));
	puts("SetRegulatorMode done");
	Radio.SetLNAGainSetting(SX128x::LNA_HIGH_SENSITIVITY_MODE);
	puts("SetLNAGainSetting done");
	Radio.SetTxParams(power, SX128x::RADIO_RAMP_20_US);
	puts("SetTxParams done");

	Radio.SetBufferBaseAddresses(0x00, 0x00);
	puts("SetBufferBaseAddresses done");


	SX128x::ModulationParams_t ModulationParams;
	SX128x::PacketParams_t PacketParams;

	if (modmode == 0) {
		ModulationParams.PacketType = SX128x::PACKET_TYPE_LORA;

		ModulationParams.Params.LoRa.Bandwidth = BW_Reg;
		std::cout << "Set BW Register 0x" << std::hex << ModulationParams.Params.LoRa.Bandwidth << "\n";
		ModulationParams.Params.LoRa.SpreadingFactor = SF_Reg;
		std::cout << "Set SF Register 0x" << std::hex << ModulationParams.Params.LoRa.SpreadingFactor << "\n";
		ModulationParams.Params.LoRa.CodingRate = CR_Reg;
		std::cout << "Set CR Register 0x" << std::hex << ModulationParams.Params.LoRa.CodingRate << "\n";

		PacketParams.PacketType = SX128x::PACKET_TYPE_LORA;
		auto &l = PacketParams.Params.LoRa;
		l.PayloadLength = 253;
		l.HeaderType = SX128x::LORA_PACKET_FIXED_LENGTH;
		l.PreambleLength = 12;
		l.Crc = SX128x::LORA_CRC_ON;
		l.InvertIQ = SX128x::LORA_IQ_NORMAL;

		Radio.SetPacketType(SX128x::PACKET_TYPE_LORA);
	} else {
		ModulationParams.PacketType = SX128x::PACKET_TYPE_FLRC;
		auto &p = ModulationParams.Params.Flrc;
		p.CodingRate = SX128x::FLRC_CR_1_2;
		p.BitrateBandwidth = SX128x::FLRC_BR_0_325_BW_0_3;
		p.ModulationShaping = SX128x::RADIO_MOD_SHAPING_BT_OFF;


		PacketParams.PacketType = SX128x::PACKET_TYPE_FLRC;
		auto &l = PacketParams.Params.Flrc;
		l.PayloadLength = 127;
		l.HeaderType = SX128x::RADIO_PACKET_VARIABLE_LENGTH;
		l.PreambleLength = SX128x::PREAMBLE_LENGTH_32_BITS;
		l.CrcLength = SX128x::RADIO_CRC_OFF;
		l.SyncWordLength = SX128x::FLRC_SYNCWORD_LENGTH_4_BYTE;
		l.SyncWordMatch = SX128x::RADIO_RX_MATCH_SYNCWORD_1;
		l.Whitening = SX128x::RADIO_WHITENING_OFF;

		Radio.SetPacketType(SX128x::PACKET_TYPE_FLRC);
	}

	puts("SetPacketType done");
	Radio.SetModulationParams(ModulationParams);
	puts("SetModulationParams done");
	Radio.SetPacketParams(PacketParams);
	puts("SetPacketParams done");

	//auto freq = strtol(argv[3], nullptr, 10);
	Radio.SetRfFrequency(freq * 1000000UL);
	puts("SetRfFrequency done");

	if (modmode == 1) {
		// only used in GFSK, FLRC (4 bytes max) and BLE mode
		uint8_t sw[] = {0xDD, 0xA0, 0x96, 0x69, 0xDD};
		Radio.SetSyncWord(1, sw);
		// only used in GFSK, FLRC
		uint8_t crcSeedLocal[2] = {0x45, 0x67};
		Radio.SetCrcSeed(crcSeedLocal);
		Radio.SetCrcPolynomial(0x0123);
//		Radio.SetWhiteningSeed(0x22);
	}

	std::cout << Radio.GetFirmwareVersion() << "\n";

	Radio.callbacks.txDone = []{
		puts("Wow TX done");
	};

	size_t pkt_count = 0;

	Radio.callbacks.rxDone = [&] {
		puts("Wow RX done");


		SX128x::PacketStatus_t ps;
		Radio.GetPacketStatus(&ps);

		uint8_t recv_buf[253];
		uint8_t rsz;
		Radio.GetPayload(recv_buf, &rsz, 253);

		uint8_t err_count = 0;

		for (size_t i=0; i<rsz; i++) {
			uint8_t correct_value;
			if (i % 2)
				correct_value = 0x55;
			else
				correct_value = 0xaa;

			if (recv_buf[i] != correct_value)
				err_count++;
		}

//		for (size_t i=0; i<rsz; i++) {
//			printf("%02x ", recv_buf[i]);
//		}
//
//		puts("");

		pkt_count++;
		printf("Packet count: %ld\n", pkt_count);

		printf("corrupted bytes: %u/%u, BER: %f%%\n", err_count, rsz, (double)err_count/rsz*100);

		if (ps.packetType == SX128x::PACKET_TYPE_LORA) {
			int8_t noise = ps.LoRa.RssiPkt - ps.LoRa.SnrPkt;
			int8_t rscp = ps.LoRa.RssiPkt + ps.LoRa.SnrPkt;
			printf("recvd %u bytes, RSCP: %d, RSSI: %d, Noise: %d, SNR: %d\n", rsz, rscp, ps.LoRa.RssiPkt, noise, ps.LoRa.SnrPkt);
		} else if (ps.packetType == SX128x::PACKET_TYPE_FLRC) {
			printf("recvd %u bytes, RSSI: %d\n", rsz, ps.Flrc.RssiSync);

		}
	};

	auto IrqMask = SX128x::IRQ_RX_DONE | SX128x::IRQ_TX_DONE | SX128x::IRQ_RX_TX_TIMEOUT;
	Radio.SetDioIrqParams(IrqMask, IrqMask, SX128x::IRQ_RADIO_NONE, SX128x::IRQ_RADIO_NONE);
	puts("SetDioIrqParams done");

	Radio.StartIrqHandler();
	puts("StartIrqHandler done");


	auto pkt_ToA = Radio.GetTimeOnAir();

	if (strcmp(argv[1], "tx") == 0) {
		uint8_t buf[253];

		for (size_t i=0; i<sizeof(buf); i++) {
			if (i % 2)
				buf[i] = 0x55;
			else
				buf[i] = 0xaa;
		}

		while (1) {


			Radio.SendPayload(buf, modmode == 0 ? 253 : 127, {SX128x::RADIO_TICK_SIZE_1000_US, 1000});
			puts("SendPayload done");

			usleep((pkt_ToA + 20) * 1000);
		}
	} else {
		Radio.SetRx({SX128x::RADIO_TICK_SIZE_1000_US, 0xFFFF});
		puts("SetRx done");

		while (1) {
			sleep(1);
		}
	}

}
