# Epsilon-NFA to DFA
An Epsilon NFA to DFA converter implemented in C
Use the NfaE_file.c along with the input files (ip1 - ip6) using input redirection.
Use the NfaE_terminal.c to type in dynamic input.

Input format:

  `no. of states`
  
  `no. of symbols`
  
  `transition of state-1 on symbol-1`
  
  `transition of state-1 on symbol-2`
  
  .
  
  .
  
  .
  
  `tranistion of state-1 on symbol-n`
  
  `lambda transition for state1`
  
  
  repeat for all states in the same format.
  
  enter "null" in case no transition exists.
