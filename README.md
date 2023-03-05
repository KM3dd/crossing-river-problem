# crossing-river-problem
crossing river problem is a good practice to understand semaphores 
# problem  description 
there is a rowboat that is used by both Linux hackers and Microsoft employees (serfs) to cross a river. The ferry can hold exactly four people; it won’t leave the shore with more or fewer. Toguarantee the safety of the passengers, it is not permissible to put one hacker in the boat with three serfs, or to put one serf with three hackers. Any other combination is safe.

As each thread boards the boat it should invoke a function calledboard. You must guarantee that all four threads from each boatload invoke board before any of the threads from the next boatload do.

After all four threads have invoked board, exactly one of them should call a function named rowBoat, indicating that that thread will take the oars. It doesn’t matter which thread calls the function, as long as one does.

Don’t worry about the direction of travel. Assume we are only interested in traffic going in one of the directions.

# how to run 
gcc creat.c -o creat -lpthread
gcc Windows.c -o windows -lpthread
gcc Hacker.c -o hacker -lpthread

execusion :
./creat
