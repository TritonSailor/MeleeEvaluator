# MeleeEvaluator
Adds an anim graph node that can process animation between frames for incredibly accurate melee traces.

This means that you could get traces that are as accurate as you can imagine while the game is only running at 10fps.

Simulation frequency on the animation graph node determines accuracy. 60 should be high enough. 120 is super precise. Anything more is probably overkill no matter how fast paced your game is.

# How it works
* MeleeEvaluatorComponent->StartMelee (pass animation)
* FAnimNode_MeleeEvaluatorNode->Plays animation in a substep->Caches transforms->Sends to FMeleeAnimInstanceProxy
* MeleeAnimInstance->Reads from FMeleeAnimInstanceProxy and calls MeleeEvaluatorComponent->OnProcessSweepTransforms
* Override in child MeleeEvaluatorComponent to perform actual sweeps (or send it back to the character to do it there)

# How to Use
Video Tutorial Coming

# Credits
Created by Vaei based on the concept by Jack Knobel
