// SPDX-License-Identifier: MIT

// File: knx_processor.h
// Author: Simon Billemont (Since 2026)

#pragma once

namespace esphome {
namespace knx {

class KnxTelegram;

class KnxProcessor {
public:
    virtual void handle_knx_telegram( KnxTelegram* telegram ) = 0;

};

}  // namespace knx
}  // namespace esphome
