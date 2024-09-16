Instances used in:

Ciriaco D’Ambrosio, Federica Laureana, Andrea Raiconi, Gaetano Vitale
The Knapsack Problem with Forfeit Sets
Submitted to Computers & Operations Research (Elsevier)
=====================================================================

Instances are organized in folders depending on the scenario (1, 2, 3, 4) and subfolders depending on the type of data correlation (not-correlated, correlated and fully correlated).

For each instance file, the structure is the following:

Line 1 contains the following values:
nI nP kS
where:
nI = number of items
nS = number of forfeit sets
kS = knapsack capacity

Line 2 contains item profits, in order of index 0...nI-1

Line 3 contains item weights, in order of index 0...nI-1

The remaining 2*nS lines contain the following values:

nA_0 fC_0 nI_0
ids_s_0
(...)  
nA_i fC_i nI_i
id_s_i
(...)  
nA_(nS-1) fC_(nS-1) nI_(nS-1)
id_s_(nS-1)

where:
nA_i = number of items allowed in the solution without having to pay forfeit cost for forfeit set i 
fC_i = forfeit cost associated to forfeit set i
nI_i = cardinality of the list of items contained in the subsequent line (forfeit set i)
id_s_i = list of items in the forfeit set i (belonging to {0,...,nI-1})