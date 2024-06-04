#include "SX128x_Linux.hpp"

// Pins based on Hunter's hardware configuration (busy, nrst, nss, dio1, dio2, dio3, tx_en, rx_en, TCXO_EN)
	SX128x_Linux :: PinConfig Hunter_pins  {26, 22, 5, 19, -1, -1, 23, 24, 25};

// Pins based on Jake's RX hardware configuration (busy, nrst, nss, dio1, dio2, dio3, tx_en, rx_en, TCXO_EN)
    SX128x_Linux :: PinConfig Jake_RX_pins  {12, 2, 6, 19, -1, -1, -1, 24, 25};

// Pins based on Jake's TX hardware configuration (busy, nrst, nss, dio1, dio2, dio3, tx_en, rx_en, TCXO_EN)
	SX128x_Linux :: PinConfig Jake_TX_pins  {26, 3, 5, 21, -1, -1, 23, -1, 25};
