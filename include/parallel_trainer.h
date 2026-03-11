#pragma once

#include "training_config.h"

#include <memory>

void parallel_train(TrainingConfig const &config,
                    std::shared_ptr<TrainingProgress> progress = nullptr);
