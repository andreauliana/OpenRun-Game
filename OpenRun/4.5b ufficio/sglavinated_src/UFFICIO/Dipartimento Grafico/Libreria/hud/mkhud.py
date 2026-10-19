# WipeOut-style accelerometer tiles + HUD recolor LUTs/tables -> hud/hud_data.s
import numpy as np
from PIL import Image
W,D,K,R=7,14,12,4          # HUD palette line 2: white, dark grey, black, red
def seg(px,base,col):
    for y in range(2,7):
        s=(6-y)//2
        for x in (base+s,base+s+1):
            px[y,x]=col
        # black edge on the right of every row for contrast on the sky
        if base+s+2<8 and px[y,base+s+2]==0: px[y,base+s+2]=K
def tile(a,b):
    px=np.zeros((8,8),int); seg(px,0,a); seg(px,4,b); return px
T={'E':tile(D,D),'H':tile(W,D),'F':tile(W,W),'HR':tile(R,D),'FR':tile(R,R)}
def enc(px): return ['0x'+''.join('%X'%v for v in row) for row in px]
out=['acc_t:']
for k in ('E','H','F'): out.append('        .long   '+','.join(enc(T[k])))
out.append('acc_r:')
for k in ('HR','FR'): out.append('        .long   '+','.join(enc(T[k])))
# recolor LUTs (byte = two pixels)
def lut(m):
    f=lambda n:m.get(n,n)
    return [(f(b>>4)<<4)|f(b&15) for b in range(256)]
L0=lut({2:K,10:K,14:K}); L1=lut({11:W}); L2=lut({4:K,8:K})
out.append('hr_lut:')
for L in (L0,L1,L2):
    for i in range(0,256,32): out.append('        .byte   '+','.join(str(v) for v in L[i:i+32]))
tiles=[(t,0) for t in list(range(0x583,0x58C))+list(range(0x593,0x59C))]      # TIME + SCORE letters
tiles+=[(t,1) for t in list(range(0x5AA,0x5AF))+list(range(0x5BA,0x5BF))]      # STAGE letters
tiles+=[(t,1) for t in list(range(0x5A1,0x5A6))+list(range(0x5B1,0x5B6))]      # big digits 1-5 (stage number)
tiles+=[(t,2) for t in (0x5F6,0x5F7,0x5F8,0x5F9,0x5FA,0x5FB,0x5FC,0x5FD,0x5FE)] # route flag
out.append('hr_tab:')
for i in range(0,len(tiles),12): out.append('        .word   '+','.join('0x%04X'%(t|(l<<12)) for t,l in tiles[i:i+12]))
out.append('        .equ HRN,%d'%len(tiles))
out.append('        .align  2')
open('hud/hud_data.s','w').write('\n'.join(out)+'\n')
pal={0:(64,64,224),W:(255,255,255),D:(73,73,73),K:(0,0,0),R:(182,0,0)}
im=Image.new('RGB',(9*8,8))
lv=11
for c in range(9):
    n=max(0,min(2,lv-2*c)); hot=c>=7
    k=['E','HR' if hot else 'H','FR' if hot else 'F'][n]
    for y in range(8):
        for x in range(8): im.putpixel((c*8+x,y),pal[T[k][y,x]])
im.resize((72*8,64),Image.NEAREST).save('hud/acc_preview.png'); print(len(tiles),'remap tiles')
