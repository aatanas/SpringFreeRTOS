# SpringFreeRTOS
A scheduler on top of FreeRTOS that allows tasks to be scheduled according to the Spring algorithm.

The scheduler is implemented as a high-priority task that schedules other tasks over the existing FreeRTOS scheduler.

The Spring algorithm supports:

- All basic heuristic functions:
  
   - a
  
   - C
  
   - d
  
   - d + W*C

- Working with resources:
  
   - T<sub>est</sub>
  
   - d + W * T<sub>est</sub>

- Restriction of precedence:
  
   - E

Jobs are assigned to the system for execution in groups (batches).
When starting each group, the Spring scheduler will receive as a parameter a series of heuristic functions that it should use during the schedule.
If the system can arrange a group according to some function it will do so. 
Otherwise it reports that it is not possible.
The schedule is not good if any work breaks its deadline.

If the schedule is feasible, the system executes jobs in that order, and writes every context substitution
in to a text file, i.e. in which tick exactly which of the jobs was started.

The system provides detailed execution logging.
Providing input to the system is possible through a configuration file, as well as from the console.

When specifying an input, the user specifies the following elements:

- Resources:
  
   - Enumerate what resources exist in the system, and for each of them which is the earliest
     time when it can be used.

- Data on jobs:
  
   - Start time.
  
   - Execution time - we assume that the user knows about each task exactly
     how long it lasts, as well as that it will not make mistakes when entering.
  
   - Deadline.
  
   - Job function, given as a string job name - the assumption is that they are
     functions fixed in the system and will not change dynamically.
  
   - The number of resources this job uses.

- Selection of a series of heuristic functions that will be used for execution.

- Restriction of precedence.



All time parameters are specified in ticks.
