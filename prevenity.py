#=============================================================================#
def ror( dword, bits ):
  return ( dword >> bits | dword << ( 32 - bits ) ) & 0xFFFFFFFF
#=============================================================================#
def hash(function, bits=13):
  hash = 0
  for c in str( function ):
    hash  = ror( hash, bits )
    hash += ord( c )

  print ("%s (0x%08X)" % ( function, hash ))
   
#=============================================================================#
