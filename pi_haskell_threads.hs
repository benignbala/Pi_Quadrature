--  Haskell implementation of Pi by Quadrature.  This uses threads to parallelize.
--
--  Copyright © 2009--2011 Russel Winder

module Main where

import Data.Time.Clock ( getCurrentTime , diffUTCTime )
import GHC.Conc ( numCapabilities )
import Control.Concurrent ( forkIO , newEmptyMVar , putMVar, takeMVar )

piIter :: Int -> Int -> Double -> Double -> Double
piIter  n to delta accumulator
    | n > to = 4.0 * accumulator * delta
    | otherwise = 
        let
          nPlus1 = n + 1
          x = ( ( fromIntegral n ) - 0.5 ) * delta
          value = accumulator + 1.0 / ( 1.0 + x * x )        
        in
          piIter nPlus1 to delta value

spawnWorkersAndSum :: Int -> Double -> Int -> IO ( Double )
spawnWorkersAndSum 0 _ _ = return ( 0.0 )
spawnWorkersAndSum i delta sliceSize = do
  forkedWorkerValue <- newEmptyMVar
  forkIO $ do
    -- Ensure strictness so as to ensure the computation actually happens in the spawned thread and not the
    -- parent thread.
    let value = piIter ( 1 +  ( i - 1 ) * sliceSize ) ( i * sliceSize )  delta 0.0
    putMVar forkedWorkerValue ( value `seq` value ) 
  y <- spawnWorkersAndSum ( i - 1 ) delta sliceSize
  x <- takeMVar forkedWorkerValue
  return ( x + y )

execute :: Int -> IO ( )
execute numberOfSlices = do
  let n = 1000000000
  let delta = 1.0 / ( fromIntegral n )
  startTime <- getCurrentTime
  let sliceSize = n `div` numberOfSlices
  pi <- spawnWorkersAndSum numberOfSlices delta sliceSize
  --  Don't get the time here since nothing has actually been computed as yet since pi has not been used.
  putStrLn ( "==== Haskell Threads pi = " ++ show pi )
  endTime <- getCurrentTime
  putStrLn ( "==== Haskell Threads iteration count = " ++ show n )
  putStrLn ( "==== Haskell Threads elapse time = " ++ show ( diffUTCTime endTime startTime ) )
  putStrLn ( "==== Haskell Threads slice count = " ++ show numberOfSlices )
  putStrLn ( "==== Haskell Threads processor count = " ++ show numCapabilities )

main :: IO ( )
main = do
  execute 1
  putStrLn ""
  execute 2
  putStrLn ""
  execute 8
  putStrLn ""
  execute 32
