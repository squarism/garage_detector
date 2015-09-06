## Garage Door Detector

_Problem: I leave my garage open all night.  I can't see my garage door from any window.  We can solve this third-world problem easily._

This is some code that sends a single event when basically
a boolean value has changed on an Arduino (particle core).

In this case, we are watching a garage door with a security
door magnet switch.  When the door is closed, the circuit is
closed so voltage flows through.

## How it works

```
Particle board --> Wifi --> Cloud (sends event)

Particle service (webhook) --> sends events to subscribers
```
You could send arbitrary data and messages to anything you want.


## TODO: 

* add code for a monitor:
  * this would let you put another particle somewhere so you could see if your garage door was open (maybe far away in a kitchen or something)


## Tools

* Particle Dev (Atom based IDE)
* Particle.io cloud publish (sends events to cloud)
