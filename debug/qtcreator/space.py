from dumper import *
from eigen import *


def qdump__vcl__BitSet(d, value):
    size = value["mBits"].type.size() * 8

    mBits = value["mBits"].integer()
    d.putValue("BitSet of size " + str(size))

    retList = []
    for i in range(size):
        bit = mBits & (1 << i)
        retList.append(('[%d]' % i, bit != 0))
    d.putExpandable()
    d.putNumChild(size)
    if d.isExpanded():
        with Children(d, size, maxNumChild=10000):
            for i in d.childRange():
                bit = mBits & (1 << i)
                if bit != 0:
                    bit = 1
                with SubItem(d, i):
                    d.putValue(bit)
                    d.putType('bool')


def qdump__vcl__Color(d, value):
    pointValue = value.members(True)[0]
    eigenValue = value.members(True)[0]
    #d.putItem(eigenValue)
    d.putValue('vcl::Color')
    d.putExpandable()
    d.putNumChild(4)
    vs = ['[Red]', '[Green]', '[Blue]', '[Alpha]']
    if d.isExpanded():
        with Children(d, 4):
            for i in d.childRange():
                v = get_ith_eigen_value(d, pointValue, i)
                with SubItem(d, i):
                    d.putName(vs[i])
                    d.putValue(str(v.integer()))


def qdump__vcl__Point(d, value):
    eigenValue = value.members(True)[0]
    d.putItem(eigenValue)
    d.putType(value.type.name)


def qdump__vcl__Quaternion(d, value):
    eigenValue = value.members(True)[0]
    d.putItem(eigenValue['m_coeffs'])
    d.putType(value.type.name)


def qdump__vcl__Vector(d, value):
    cont = value["mContainer"]
    d.putItem(cont)
    d.putType(value.type.name)
