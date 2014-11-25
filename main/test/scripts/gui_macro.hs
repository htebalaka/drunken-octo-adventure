
import Prelude
import Control.Applicative
import Control.Monad
import Data.List (isPrefixOf)
import System.Directory (getDirectoryContents)
import qualified System.Process as P

-- returns all `runMacro` files
getMacros :: IO [String]
getMacros = go <$> getDirectoryContents "../data" where
    go :: [String] -> [String]
    go = map (directory++) . filter (isPrefixOf "macro")
    directory :: String
    directory = undefined

-- takes 2 files to compare, and writes their diffs to a third file
writeDiff :: String -> String -> String -> IO String
writeDiff file1 file2 fileOut = do
    contents <- P.readProcess "diff" [file1, file2] ""
    unless (null contents) $ writeFile fileOut contents
    return contents

-- runs a test using the given macro file, the record_{} strings indicate
-- the files that will be used to write test data, the run_{} strings indicate
-- the files that will be used to read in test data, and the diff_{} strings
-- indicate where the diffs will be written
runTest :: String -> IO ()
runTest path =
    let recordMacro = undefined -- file for currently recorded input
        recordStdout = undefined -- file for currently recorded stdout
        runMacro = path
        runStdout = undefined -- file for previously recorded stdout
        diffMacro = undefined
        diffStdout = undefined
    in do
        _ <- P.readProcess "bash" [] $ "make gui; ./gui.out " ++ runMacro ++  " " ++ recordMacro ++ " " ++ recordStdout
        putStrLn =<< writeDiff runMacro recordMacro diffMacro
        putStrLn =<< writeDiff runStdout recordStdout diffStdout

main :: IO ()
main = mapM_ runTest =<< getMacros

