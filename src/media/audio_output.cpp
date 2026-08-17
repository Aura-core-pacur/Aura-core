/*
 * Aura Core
 * Copyright (C) 2026 Aura-core-pacur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "audio_output.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>

AudioOutput::AudioOutput() = default;

AudioOutput::~AudioOutput() {
    stop();
}

void AudioOutput::start() {
    running_ = true;
    thread_ = std::thread([this]() {
        while (running_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            if (clock_ != nullptr && video_pts_ >= 0.0) {
                if (std::abs(clock_->current_pts - video_pts_) > 0.05) {
                    clock_->current_pts = video_pts_;
                }
            }
        }
    });
}

void AudioOutput::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void AudioOutput::set_video_pts(double pts) {
    video_pts_ = pts;
}

void AudioOutput::set_clock(AudioClock* clock) {
    clock_ = clock;
}
