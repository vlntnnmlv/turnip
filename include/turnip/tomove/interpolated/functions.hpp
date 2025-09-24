#pragma once

namespace turnip {
/* ----- The transition functions -----
   The equations can be found on easings.net and in the GitHub repo
*/
float Linear(float t);
float EaseInOutExponential(float t);
float EaseOutBack(float t);
float EaseInBack(float t);
float EaseOutElastic(float t);

/// The currently supported transition functions
enum class TransitionFunction {
    None,
    Linear,
    EaseInOutExponential,
    EaseOutBack,
    EaseInBack,
    EaseOutElastic,
};

/// Calls the easing function associated with the provided enum entry
float GetRatio(float t, TransitionFunction transition);
}
