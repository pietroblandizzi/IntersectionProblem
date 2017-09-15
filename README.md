# IntersectionProblem
Task from an interview and my solution in c++


=============================City Problem===============================

Imagine a city and everything related to traffic within itself, e.g. 

vehicles,roads, traffic signs, pedestrians, bicycles... and think about 

the relationship between each other or how could impact the difference 

of two objects of the same type, e.g. a car and a truck. Think what is 

characteristic from each of them and what they share in common some of 

them. 



Your task is to create the different classes to be used in a software 

that will simulate such scenario. The methods should be define but not 

implemented. You can include and use anything from the C++ Standard 

Library (we encourage to do so actually), but there are some 

requirements:

 - At least one template has to be defined

 - At least one abstract base class which is inherited



The expected result is a series of header files.

========================================================================

Question suggestion to city problem: 

- Imagine that there is a cross junction as shown below, where each vehichle

  arrived to the cross must yield to the vehicle on the right, if there is already 

  one there. Furthermore, two vehicles are not allowed to pass through the cross 

  at the same time. Please, implement a solution for this scenario. 

  What can be a problem? 

     

- Now assume that more than one vehicle is allowed to enter  the cross at the same time.

  For example, a car coming from direction 2, that wants to turn left, will pass through 

  the quadrants B, A and finally D. 

  Can you implement a solution also for this scenario?

  

  

			1   

        | v | ^ |                   

        | v | ^ |

        | v | ^ |

 --------------------------

   <<<< | A + B | <<<< 

 4 -----  + + +  -------- 2

   >>>> | D + C | >>>>

 --------------------------

        | v | ^ |

        | v | ^ |

        | v | ^ |
