[Python标准库]re——正则表达式
    作用：使用形式化模式搜索和修改文本。
    Python 版本：1.5 及以后版本
    正则表达式（regular expression）是用一种形式化语法描述的文本匹配模式。模式被解释为一组命令，以一个字符串作为输入，生成一个匹配的子集或原字符串的修改版本。“正则表达式”一词在讨论中通常会简写为“regex”或“regexp”。表达式可以包括字面量文本匹配、重复、模式组合、分支以及其他复杂的规则。对于很多解析问题，用正则表达式解决会比创建特殊用途的词法分析器和语法分析器更为容易。
    正则表达式通常在涉及大量文本处理的应用中使用。例如，在开发人员使用的文本编辑程序中（包括 vi、emacs 和其他现代 IDE）常用正则表达式作为搜索模式。另外，正则表达式还是 UNIX 命令行工具的一个不可缺少的部分，如 sed、grep 和 awk。很多编程语言都在语言语法中包括对正则表达式的支持，如 Perl、Ruby、Awk 和 Tcl。另外一些语言（如 C、C++ 和 Python）则通过扩展库来支持正则表达式。
    有很多开源的正则表达式实现，这些实现都有一种共同的核心语法，不过对其高级特性有不同的扩展或修改。Python 的 re 模块中使用的语法以 Perl 所用正则表达式语法为基础，并提供了一些特定于 Python 的改进。
查找文本中的模式
    re 最常见的用法就是搜索文本中的模式。search() 函数取模式和要扫描的文本作为输入，如果找到这个模式则返回一个 Match 对象。如果未找到模式，search() 将返回 None。
    每个 Match 对象包含有关匹配性质的信息，包括原输入字符串、使用的正则表达式，以及模式在原字符串中出现的位置。

import re

pattern = 'this'
text = 'Does this text match the pattern?'

match = re.search(pattern, text)

s = match.start()
e = match.end()

print 'Found "%s"\nin "%s"\nfrom %d to %d ("%s")' % \
    (match.re.pattern, match.string, s, e, text[s:e])

编译表达式
    re 包含一些模块级函数，用于处理作为文本字符串的正则表达式，不过对于程序频繁使用的表达式，编译这些表达式会更为高效。compile() 函数会把一个表达式字符串转换为一个 RegexObject。

import re

# Precompile the patterns
regexes = [ re.compile(p) for p in ['this', 'that']]

text = 'Does this text match the pattern?'

print 'Text: %r\n' % text

for regex in regexes:
	print 'Seeking "%s" ->' % regex.pattern,

	if regex.search(text):
		print 'match!'
	else:
		print 'no match'

    模块级函数会维护已编译表达式的一个缓存。不过，这个缓存的大小是有限的，直接使用已编译表达式可以避免缓存查找的开销。使用已编译表达式的另一个好处是，通过在加载模块时预编译所有的表达式，可以把编译工作转到应用开始时，而不是当程序响应一个用户动作时才进行编译。
多重匹配
    到目前为止，示例模式都使用 search() 来查找字面量文本字符串的单个实例。findall() 函数会返回输入中与模式匹配而不重叠的所有子串。

import re

text = 'abbaaabbbbaaaaa'

pattern = 'ab'

for match in re.findall(pattern, text):
	print 'Found "%s"' % match

    这个输入字符串中有两个 ab 实例
    finditer() 会返回一个迭代器，它将生成 Match 实例，而不像 findall() 返回字符串。

import re

text = 'abbaaabbbbaaaaa'

pattern = 'ab'

for match in re.finditer(pattern, text):
	s = match.start()
	e = match.end()
	print 'Found "%s" at %d:%d' % (text[s:e], s, e)

    这个例子找到了 ab 的两次出现，Match 实例显示出它们在原输入字符串中的位置。
