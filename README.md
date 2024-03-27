# Docker From Scratch

docker from scratch following the codecrafters challenge, this is an educational project intended to better understand C and how docker works.

## Notes

### On Stage 1 and 2

- code worked with little modification but this was a
  quirk of the testing and not intended.

- It took me a long time to understand the execv command,
  images and pipes, specifically stderr and stdout.

### Stage 3

- This one took a while and I think a C programmer would have done it in 5 mins.
  In the end I had to look up the solution, I nearly
  had the answer but failed to return the correct
  value at the end of the if statement.

### Stage 4

- Im really struggling now and get the feeling
  a C developer would have no issues here.
  In the end I had to use one of the examples provided
  I tried using the command line tools like.
  cp and mkdir but It wasnt giving the expected
  output for the tests.

- In the end I get the feeling, for me at least
  this would be much easier to do in a bash script
  using command line tools. This is probably
  due to my lack of familiarity with
  C unix programming.

- overall as much as im learning here
  much of it is just a respect for C programming
  I can see clearly the gulp in my understanding
  and what is required to be proficient.

- I also get the feeling this is a simply task
  in C, just spawning a process, attaching pipes,
  making it secure chroot. I know this would be
  very straight forward to accomplish in bash

### Final thoughts

- This challenge proved too much for me, unfortunately languages like Haskell or C++
  were not available I choose C as I thought I could get away with it
  going into it I thought it would be a simply case of
  chroot and isolating the enviroment
  I thought I could lean heavily on my
  knowledge of linux and bash scripting
  in the end my lack of ability in C was stark

- To get through the challenges I spent
  a lot of time googling looking at other peoples
  code and hacking solutions.

- I abused the limited testing for the challenge
  and the fact I was developing on Debian, and
  made the output match what was expected.

- I still did learn a lot though,
  about cloning processes, setting up pipes
  handling stdout and stderr
  I learned about using commadline tool
  in C and a number of useful functions in C

- In the final task it was expteced to
  handle authentication and interact with docker.io

- I think doing this as expected would have
  taken 10 times as long as I alread spent on it
  It would have been a really deep dive into C networking
  The debugging alone would have been hours of work.
  My goal was to become more familiar at at high level
  with C linux and docker. I want to get back to C++ and Haskell
  I might revisit this challenge in the future but probably with a
  higher level language with better networking support.

- Overall this project on codecrafters required more time
  and effort than I was will to invest in C right now. It was
  tempting but my goal now is to push my fluency in Haskell to
  match my C++ and a tangent into C wont offer much for my
  current projects.

- Still at the end I learned a lot Nd it made me think about
  how I would write C++ or Haskell to interact with C.
