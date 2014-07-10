/* This file works on the linear ILC produced by ILCLinear, and applies various optimizations
 * to reduce the number of registers the naive linear ILC uses.
 *
 * The first processes the ILC, and looks at each time a variable is stored into a register
 * When the variable is used again, it determines either 
