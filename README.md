# Pico ADC Acquisition trough SPI Transmission

This project implements real-time data acquisition on a Raspberry Pi Pico using a 16-bit parallel ADC (ADS8405).  
Samples are stored in a double-buffer in Pico's RAM and transmitted over SPI to a Raspberry Pi.

## Features

- 1 MSPS 16-bit ADC sampling
- Double-buffered acquisition
- Transmission over SPI at high speed
- Optional DMA-based SPI transfer

## Requirements

- Raspberry Pi Pico (RP2040)
- ADS8405 16-bit ADC
- CMake and Pico SDK
- Raspberry Pi (or equivalent) as SPI slave receiver
