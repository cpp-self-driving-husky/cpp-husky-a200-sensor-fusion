# CPP Husky A200 Sensor Fusion

## Introduction

Robotic systems understand their environment through the use of a sensor suite. It is necessary to to fuse the output between sensors and across time points in a process called sensor fusion (also termed multi-sensor data fusion). Specifically, sensor fusion is the combining of data immediately from or data derived from sensors in order to produce enhanced data with the intent of reducing ambiguity and uncertainty, extending temporal and spatial coverage, and improving resolution of the robotic system.

Given the inherent noise of robotic sensors, uncertainty is ubiquitous in the sensing and fusion processes. Probabilistic models are thus employed to use the available information to its maximum effect. Almost all data fusion problems involve an estimation process, wherein a number of observations from multiple sensors are obtained to make inferences about the true state of the environment under observation.  

The most common algorithm utilized in this problem is the Kalman filter.  It consists of recursive, two-step cycles alternating between predicting the current state, then updating the state estimation. This filter fuses data from successive time intervals to provide estimates of the state. For example, smoothed estimates of position, velocity, and acceleration can be elucidated. Additionally, data from multiple sensors may be fused into a vector of internal states, under the condition that the input and system states are linearly dependent. Alternate filters, including the extended and unscented Kalman filter, among others, have been proposed which do not require assumptions of linearity.


## User Stories

As a user, I would like gather data from robotic sensors across time points to minimize uncertainty in sensor readings.

As a user, I would like to use complementary, competitive, or cooperative sensor networks to ensure measurements are as optimal as possible.

As a user, I would like to use multiple sensors to gather complementary data in the instance that one sensor is inaccurate, or in the circumstance that a sensor fails.


## Requirements

The vehicle must be able to integrate input from multiple proprioceptive sensors to determine internal state such as position, orientation, etc...

The vehicle must fuse data measured at discrete time intervals to filter signals with a reasonable volume of statistical error.

The current state of the vehicle must be estimated based on the history of measurements from previous time intervals.


## Solution

Given that sensor fusion paradigms depend heavily on the application in which they are employed, it is difficult to create generalized models across robotic architectures. Thus, sensor fusion architectures must be explored and optimized in the context of the A200 husky (and later, self-driving cart). The LAAS architecture provides a promising series of abstractions for use in robotic systems, and thus warrants further exploration in this design.

The numerous filtering algorithms must be explored.  Kalman filtering is a common, well-tested technique in data fusion, though may be ineffective if sensor uncertainty does not follow a Gaussian distribution. Alternative techniques exist, such as extended Kalman filters, unscented Kalman filters, particle filters, etc.. While more accurate, these filters come at the expense of increased computational complexity and thus warrant testing and exploration in the husky's paradigm.

Initial implementation of the filtering methods will likely be written in Python, as it allows rapid testing in understandable code. Refinement and optimization will be completed in C++, as efficiency is essential for these computationally expensive tasks.


## Reference

A ['gentle' introduction](https://www.researchgate.net/profile/Wilfried_Elmenreich/publication/267771481_An_Introduction_to_Sensor_Fusion/links/55d2e45908ae0a3417222dd9.pdf) to sensor fusion

A quantitative sensor data fusion tutorial
[in the form of slides](http://www.acfr.edu.au/pdfs/training/IntroDataFusionSlides.pdf)
and
[a more thorough explanation](http://www.acfr.usyd.edu.au/pdfs/training/multiSensorDataFusion/dataFusionNotes.pdf)
 of said tutorial

[Kalman filter, extended Kalman filter](http://users.isr.ist.utl.pt/~mir/pub/kalman.pdf)
and [unscented Kalman filter](https://www.seas.harvard.edu/courses/cs281/papers/unscented.pdf)

[A comparison of the extended Kalman filter and particle filter algorithms](http://ac.els-cdn.com/S0378475410001515/1-s2.0-S0378475410001515-main.pdf?_tid=14b9322a-371a-11e7-9b55-00000aacb361&acdnat=1494597261_d5f7fd8ed677ca8729413833fc36be2f)
