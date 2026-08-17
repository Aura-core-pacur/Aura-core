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

#ifndef AURA_AUDIO_OUTPUT_H
#define AURA_AUDIO_OUTPUT_H

#include <atomic>
#include <thread>

struct AudioClock {
    double current_pts = 0.0;
};

class AudioOutput {
public:
    AudioOutput();
    ~AudioOutput();

    void start();
    void stop();
    void set_video_pts(double pts);
    void set_clock(AudioClock* clock);

private:
    std::thread thread_;
    std::atomic_bool running_{false};
    AudioClock* clock_ = nullptr;
    double video_pts_ = 0.0;
};

#endif
