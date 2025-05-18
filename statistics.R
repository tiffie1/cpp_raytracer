cat("Rendering of a 500x500 scene using supersampling of 25 rays per pixel.")
cat("20 renders were done, both before and after cache optimization.")
cat("The execution time in seconds was recorded below.")

before = c(
  3.939,
  4.777,
  3.948,
  3.766,
  3.767,
  3.952,
  3.785,
  3.843,
  3.838,
  3.79,
  3.78,
  3.734,
  3.86,
  3.815,
  3.954,
  3.796,
  3.777,
  3.83,
  3.806,
  3.87
)

after = c(
  3.488,
  3.48,
  3.54,
  3.814,
  3.554,
  3.502,
  3.527,
  3.535,
  3.561,
  3.621,
  3.551,
  3.567,
  3.592,
  3.568,
  3.615,
  3.579,
  3.524,
  3.561,
  3.528,
  3.63
)

cat("Before mean:", mean(before))
cat("After mean:", mean(after))
cat("There was a decrease in average running time by 0.32 seconds.")