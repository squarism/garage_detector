## Garage Door Detector

_Problem: I leave my garage open all night.  I can't see my garage door from any window.  We can solve this first-world problem easily._

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

```
Monitor --> subscribes to events (turns on status light)
```

I put the status light in a common place so everyone can see before they
go to bed whether the garage is open.  _TODO_: Pics and blog post to
illustrate.


## Tools

* Particle Dev (Atom based IDE)
* Particle.io cloud publish (sends events to cloud)
