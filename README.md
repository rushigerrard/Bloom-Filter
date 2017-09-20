# Bloom-Filter : A bloom filter is a space-efficient probabilistic data structure that is used to test whether an element is a member of a set or not.  Bloom filters and their variants are of prime importance, and they are heavily used in various distributed systems. 

For example, Cassandra uses bloom filters to test if any of the SSTables is likely to contain the requested partition key or not, without actually having to read their contents (and thus avoiding expensive IO operations).

With Bloom filters, false positives are possible but false negatives are not. Which means that if a bloom filter returns false for a given partition key, then it is absolutely certain that the partition key is not present in the corresponding SSTable; if it returns true, however, then the SSTable is likely to contain the partition key. When this happens, Cassandra will resort to more sophisticated techniques to determine if it needs to read that SSTable or not.

Here, I have implemented a structure that maintains a Bloom Filter for 2,000,000 strings with an error rate of 0.05%, using only 2Mbyte of memory.

