#!/bin/sh

bdffont="$1"
outputpng="$2"

pbmtext -font "$bdffont" << "EOF" \
    | pnmcut -left 13 -right -15 -top 7 -bottom -8 \
    | pnmtopng \
    > $outputpng
 !"#$%&'
()*+,-./
01234567
89:;<=>?
@ABCDEFG
HIJKLMNO
PQRSTUVW
XYZ[\]^_
`abcdefg
hijklmno
pqrstuvw
xyz{|}~
EOF

