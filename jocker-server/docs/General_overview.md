# General overview

## Getting started

This document describes the main features and overall structure of the program.

## About repository

Jokerd is the server-side application providing containerization functionality of the Joker project.

## Features

Joker is a tool for containerization. 
Containerization is a way of virtualizing computer's resources for one or more programs by encapsulating them inside a container abstraction.
The container abstraction is responsible for isolating the program from the rest of the system, as well as restricting the resources usage.
That is, by placing a program instance inside a container, we can ensure its proper and safe functioning by adjusting this container's settings.
Moreover, we can establish a way of communication between encapsulated programs - so we can always create an environment we want to.

Joker provides all the virtualization possibilities by utilizing the kernel-provided mechanisms, namely it relies on the Cgroups and Namespaces features.

## Jockerd structure

![Joker-structure.png](images%2FJoker-structure.png)

