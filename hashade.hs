import Control.Monad
import Data.Word
import System.IO

shvalType :: Word32 -> [Char]
shvalType _ = "invalid type "

shtexType :: Word32 -> [Char]
shtexType _ = "invalid texture "

shbufType :: Word32 -> [Char]
shbufType _ = "invalid buffer "

data Uniform = VU Word32 [Char] | BU Word32 [Char] | 
	TU Word32 [Char] deriving (Eq)

instance Show Uniform where
	show (VU x n) = show ("uniform " ++ (shvalType x) ++ n ++ ";")
	show (BU x n) = show ("uniform " ++ (shbufType x) ++ n ++ ";")
	show (TU x n) = show ("uniform " ++ (shtexType x) ++ n ++ ";")
	show _ = show ( "invalid uniform\n")

type UniformList = [Uniform]

data Attribute = PT [Char] | TT [Char] | NT [Char] | 
	VT Word32 [Char] deriving (Eq)

instance Show Attribute where
	show (PT a) = show ("in vec3 " ++ a ++ ";") 
	show (TT a) = show ("in vec2 " ++ a ++ ";")
	show (NT a) = show ("in vec3 " ++ a ++ ";")
	show (VT v a) = 

type AttributeList = [Attribute]

data Instruction = Dec [Char] [Char] | Call [Char] [[Char]] | NOP deriving (Show)

type InstructionList = [Instruction]

data Shader = GLSL 
	{
		glslv :: Word16,
		unis :: UniformList,
		atts :: AttributeList,
		src :: InstructionList
	} | Null deriving (Show)

class ShadeUnit a where
	addUnit :: Shader -> a -> Shader
	rmUnit :: Shader -> a -> Shader
	
instance ShadeUnit Uniform where
	addUnit (GLSL v u t s) x = GLSL v (u ++ [x]) t s
	addUnit Null x = Null
	rmUnit (GLSL v u t s) x = GLSL v (filter (/= x) u) t s

instance ShadeUnit Attribute where
	addUnit (GLSL v u t s) x = GLSL v u (t ++ [x]) s
	addUnit Null x = Null
	rmUnit (GLSL v u t s) x = GLSL v u (filter (/= x) t) s

