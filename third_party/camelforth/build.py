from third_party.zmac.build import zmac
from glob import glob

zmac(
    name="camelforth",
    src="./camel80.z80",
    deps=glob("third_party/camelforth/*.inc"),
    relocatable=False,
)
