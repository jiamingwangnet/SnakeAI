#pragma once

#include <vector>
#include <ReinforcementLearning/DQNAgent.h>

// Environment
static constexpr int BOARD_WIDTH = 32;
static constexpr int BOARD_HEIGHT = 24;

static constexpr int SNAKE_START_LENGTH = 3;

static constexpr int UPDATE_DELAY = 1;

// Agent
//                   output      input        activation (relu, linear, sigmoid, softmax)
#define LAYER_INFO  { 11,           0,        relu}, \
					{ 256,         11,        relu },  \
					{ 256,        256,        relu }, \
					{ 3,          256,        linear } \

static constexpr int REPLAY_SIZE = 100000;

// 0 < EPSILON_DECAY_FACTOR < 1
// lower values decay faster
static constexpr double EPSILON_DECAY_FACTOR = 0.9998;      