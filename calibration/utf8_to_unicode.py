# -*- coding: utf-8 -*
 
def to_unicode(string):
 
    ret = ''
    for v in string:
        ret = ret + hex(ord(v)).upper().replace('0X', '\\u')
 
    return ret

if __name__ == "__main__":

	#print(to_unicode("往左来点"), len(to_unicode("往左来点")))
	#print(to_unicode("往右来点"))
	#print(to_unicode("往上来点"))
	#print(to_unicode("往下来点"))
	print(to_unicode("左右"))
	print(to_unicode("前后"))
	print(to_unicode("上下"))
	print(to_unicode("距离"))
	#print(to_unicode("畸变指标"))
