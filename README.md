# Particle simulator

This project is a multithreaded particle simulator that uses C++. The user can add particles and walls into a 1280x720 canvas. These particles can bounce off the balls.

## Getting Started
1. Clone the repository
2. Install QT 6.6 [here](https://www.qt.io/download-open-source)
3. Open the project from QT
4. Run the program


## Features
The user can select between 4 modes when creating the balls. 

| Mode   | Description                                                                                                 |
| ------ | ----------------------------------------------------------------------------------------------------------- |
| Mode 1 | Create one particle at a time.                                                                              |
| Mode 2 | Create many particles at once while distributing them equally according to the start and end positions of the ball.  |
| Mode 3 | Create many particles at once while distributing them equally according to the start and end angle of the ball .     |
| Mode 4 | Create many particles at once while distributing them equally according to the start and end velocities of the ball. |

Below is the table of settings that the user can make per mode when creating the particles:
| Mode 1                | Mode 2                | Mode 3                | Mode 4                  |
| --------------------- | --------------------- | --------------------- | ----------------------- |
| Starting Position (X) | Number of balls (n)   | Number of balls (n)   | Number of balls (n)     |
| Starting Position (Y) | Starting Position (X) | Starting Position (X) | Starting Position (X)   |
| Velocity (p/s)        | Starting Position (Y) | Starting Position (Y) | Starting Position (Y)   |
| Angle (θ)             | Ending Position (X)   | Starting Angle (θ)    | Starting Velocity (p/s) |
|                       | Ending Position (Y)   | Ending Angle (θ)      | Ending Velocity (p/s)   |
|                       | Angle (θ)             | Velocity (p/s)        | Angle (θ)               |
|                       | Velocity (p/s)        |                       |                         |

Before creating the wall, the start and end positions need to be setup to define length of the wall across the canvas. Here are the list of settings the user can modify when creating the walls:

- Start (X1)
- Start (Y1)
- End (X2)
- End (Y2)

## Authors
Lim, Aurelius Justin

Yongco, Denzel Bryan
