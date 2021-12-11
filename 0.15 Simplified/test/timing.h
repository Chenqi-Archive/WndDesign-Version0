#pragma once

// Initialize timer.
void init_timing();

// Start timing. Must initialize first.
void start_timing();

// End timing and returns time elapsed in sec.
// One could start once and end multiple times.
double end_timing();