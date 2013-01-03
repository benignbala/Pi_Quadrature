--  Haskell implementation of π by Quadrature.  Parallel version using parMap.
--
--  Copyright © 2009–2011, 2013  Russel Winder

module Main where

import Control.Parallel.Strategies (rwhnf , parMap)

import Output (outn)

piIter :: Int -> Int -> Double -> Double -> Double
piIter  n to delta accumulator
    | n > to = 4.0 * delta * accumulator
    | otherwise = piIter (n + 1) to delta (accumulator + 1.0 / (1.0 + x * x))
    where
      x = ((fromIntegral n) - 0.5) * delta

piQuadSlice :: Double -> Int -> Int -> Double
piQuadSlice delta sliceSize index = piIter start end delta 0.0
    where
      start = 1 +  index * sliceSize
      end =  (index + 1) * sliceSize

execute :: Int -> IO ()
execute numberOfSlices = do
  let n = 1000000000
  let delta = 1.0 / (fromIntegral n)
  startTime <- getCurrentTime
  let sliceSize = n `div` numberOfSlices
  let pi = sum (parMap rwhnf (piQuadSlice delta sliceSize) [0 .. (numberOfSlices - 1)])
  outn "ParMap" pi n numberOfSlices

main :: IO ()
main = do
  execute 1
  execute 2
  execute 8
  execute 32
