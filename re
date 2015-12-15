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
模式语法
    正则表达式支持更强大的模式，而不只是简单的字面量文本字符串。模式可以重复，可以锁定到输入中不同的逻辑位置，还可以采用紧凑形式表示而不需要在模式中提供每一个字面量字符。使用所有这些特性时，需要结合字面量文本值和元字符（metacharacter），元字符是 re 实现的正则表达式模式语法的一部分。

# re_test_patterns.py

import re

def test_patterns(text, patterns=[]):
	"""Given source text and a list of patterns, look for 
	matches for each pattern within the taxt and print
	them to stdout.
	"""

	# Look for each pattern in the text and print the results
	for pattern, desc in patterns:
		print 'Pattern %r (%s)\n' % (pattern, desc)
		print '  %r' % text
		for match in re.finditer(pattern, text):
			s = match.start()
			e = match.end()
			substr = text[s:e]
			n_backslashes = text[:s].count('\\')
			prefix = '.' * (s + n_backslashes)
			print '  %s%r' % (prefix, substr)
		print
	return

if __name__ == '__main__':
	test_patterns('abbaaabbbbaaaaa', [('ab', "'a' followed by 'b'"),])

    上面的例子使用 test_patterns() 来研究模式变化如何影响以何种方式匹配相同的输入文本。输出显示了输入文本以及输入中与模式匹配的各个部分的子串区间。
    1. 重复
    模式中有 5 中表达重复的方式。如果模式后边跟有元字符 “*”，这个模式会重复 0 次或多次。如果将 “*” 替换为 “+”，那么这个模式必须至少出现 1 次。使用 “?” 意味着模式要出现 0 或 1 次。如果希望出现特定的次数，需要在模式后边使用 {m}，这里 m 是模式需要重复的次数。最后，如果允许重复次数可变但是有一个限定范围，可以使用 {m,n}，这里 m 是最小重复次数，n 是最大重复次数。如果省略 n（即 {m,}），表示这个值至少要出现 m 次， 但无上限。

from re_test_patterns import test_patterns

test_patterns(
	'abbaabbba',
	[ ('ab*', 'a followed by zero or more b'),
	  ('ab+', 'a followed by one or more b'),
	  ('ab?', 'a followed by zero or one b'),
	  ('ab{3}', 'a followed by three b'),
	  ( 'ab{2,3}', 'a followed by two to three b'),
	])

    ab* 和 ab? 的匹配模式要多于 ab+ 的匹配。
    正常情况下，处理重复指令时，re 匹配模式时会利用（consume）尽可能多的输入。这种所谓“贪心”的行为可能导致单个匹配减少，或者匹配中包含了多于原先预计的输入文本。在重复指令后面加上 “?” 可以关闭这种贪心行为。

from re_test_patterns import test_patterns

test_patterns(
	'abbaabbba',
	[ ('ab*?', 'a followed by zero or more b'),
	  ('ab+?', 'a followed by one or more b'),
	  ('ab??', 'a followed by zero or one b'),
	  ('ab{3}?', 'a followed by three b'),
	  ( 'ab{2,3}?', 'a followed by two to three b'),
	])

    2. 字符集
    字符集（character set）是一组字符，包含可以与模式中相应位置匹配的所有字符。例如，[ab] 可以匹配 a 或 b。

from re_test_patterns import test_patterns

test_patterns(
	'abbaabbba',
	[ ('[ab]', 'either a or b'),
	  ('a[ab]+', 'a followed by one or more a or b'),
	  ('a[ab]+?', 'a followed by one or more a or b, not greedy'),
	])

    表达式 (a[ab]+) 的贪心形式会利用整个字符串，因为第一个字母是 a，而且后面的每一个字符要么是 a 要么是 b。
    字符集还可以用来排除某些特定字符。尖字符（^）表示要查找未在随后的字符集中出现的字符。

from re_test_patterns import test_patterns

test_patterns(
	'This is some text -- with punctuation.',
	[ ('[^-. ]+', 'sequences without -, ., or space'),
	])

    这个模式将找到不包含字符 “-”、“.” 或空格的所有子串。
    随着字符集变大，键入每一个应当（或不应当）匹配的字符会变得很枯燥。可以使用一种更紧凑的格式，利用字符区间（character range）来定义一个字符集，其中包括一个起点和一个终点之间所有连续的字符。

from re_test_patterns import test_patterns

test_patterns(
	'This is some text -- with punctuation.',
	[ ('[a-z]+', 'sequences of lowercase letters'),
	  ('[A-Z]+', 'sequences of uppercase letters'),
	  ('[a-zA-Z]+', 'sequences of lowercase or uppercase letters'),
	  ('[A-Z][a-z]+', 'one uppercase followed by lowercase'),
	])

    这里的区间 a-z 包括所有小写 ASCII 字母，区间 A-Z 包括全部大写 ASCII 字母。这些区间还可以合并为一个字符集。
    作为字符集的一种特殊情况，元字符 “.”（点号）指模式应当匹配该位置的任何单字符。

from re_test_patterns import test_patterns

test_patterns(
	'abbaabbba',
	[ ('a.', 'a followed by any one character'),
	  ('b.', 'b followed by any one character'),
	  ('a.*b', 'a followed by anything, ending in b'),
	  ('a.*?b', 'a followed by anything, ending in b'),
	])

    结合点号与重复可以得到非常长的匹配结果，除非使用非贪心形式。
    3. 转义码
    还有一种更为紧凑的表示，可以对一些预定义的字符集使用转义码。re 可以识别的转义码如下所示。
    \d：一个数字
    \D：一个非数字
    \s：空白符（制表符、空格、换行符等）
    \S：非空白字符
    \w：字母数字
    \W：非字母数字
    注意：转义字符通过在该字符前面加一个反斜线（\）前缀来表示。遗憾的是，正常的 Python 字符串中反斜线自身也必须转义，这会导致表达式很难阅读。通过使用“原始”（raw）字符串（在字面值前面加一个前缀 r 来创建），可以消除这个问题，并维持可读性。

from re_test_patterns import test_patterns

test_patterns(
	'A prime #1 example!',
	[ (r'\d+', 'sequence of digits'),
	  (r'\D+', 'sequence of nondigits'),
	  (r'\s+', 'sequence of whitespace'),
	  (r'\S+', 'sequence of nonwhitespace'),
	  (r'\w+', 'alphanumeric characters'),
	  (r'\W+', 'nonalphanumeric'),
	])

    以上示例表达式结合了转义码和重复，来查找输入字符串中的类似字符序列。
    要匹配属于正则表达式语法的字符，需要对搜索模式中的字符进行转义。

from re_test_patterns import test_patterns

test_patterns(
	r'\d+ \D+ \s+',
	[ (r'\\.\+', 'escape code'),
	])

    这个例子中的模式对反斜线和加号字符进行了转义，因为作为元字符，这两个字符在正则表达式都有特殊的含义。
    4. 锚定
    除了描述要匹配的模式的内容外，还可以使用锚定（anchoring）指令指定输入文本中的模式应当出现的相对位置。下面列出了合法的锚定码。
    ^：字符串或行的开始
    $：字符串或行的结束
    \A：字符串的开始
    \Z：字符串的结束
    \b：一个单词开头或末尾的空串
    \B：不在一个单词开头或末尾的空串

from re_test_patterns import test_patterns

test_patterns(
	'This is some text -- with punctuation.',
	[ (r'^\w+', 'word at start of string'),
	  (r'\A\w+', 'word at start of string'),
	  (r'\w+\S*$', 'word near end of string, skip punctuation'),
	  (r'\w+\S*\Z', 'word near end of string, skip punctuation'),
	  (r'\w*t\w*', 'word containing t'),
	  (r'\bt\w+', 't at start of word'),
	  (r'\w+t\b', 't at end of word'),
	  (r'\Bt\B', 't, not start or end of word'),
	])

    这个例子中，匹配字符串开头和末尾单词的模式是不同的，因为字符串末尾的单词后面有一个结束句子的标点符号。模式 \w+$ 不能匹配，因为 “.” 并不是一个字母数字字符。
限制搜索
    如果提前已经知道只需搜索整个输入的一个子集，可以告诉 re 限制搜索范围，从而进一步约束正则表达式匹配。例如，如果模式必须出现在输入的最前面，那么使用 match() 而不是 search() 会锚定搜索，而不必在搜索模式中显式地包含一个锚。

import re

text = 'This is some text -- with punctuation.'
pattern = 'is'
print 'Text :', text
print 'Pattern :', pattern

m = re.match(pattern, text)
print 'Match :', m
s = re.search(pattern, text)
print 'Search :', s

    由于字面量文本 is 未出现在输入文本最前面，因此使用 match() 无法找到它。不过，这个序列在文本中另外出现了两次，所以 search() 可以找到它。
    已编译正则表达式的 search() 方法还接受可选的 start 和 end 位置参数，将搜索限制在输入的一个子串中。

import re

text = 'This is some text -- with punctuation.'
pattern = re.compile(r'\b\w*is\w*\b')

print 'Text :', text
print

pos = 0
while True:
	match = pattern.search(text, pos)
	if not match:
		break
	s = match.start()
	e = match.end()
	print '  %2d : %2d = "%s"' % (s, e-1, text[s:e])
	# Move forward in text for the next search
	pos = e

    这个例子实现了 iterall() 的一种不太高效的形式，每次找到一个匹配时，该匹配的结束位置将用于下一次搜索。
用组解析匹配
    搜索模式匹配是正则表达式所提供强大功能的基础。为模式增加组（group）可以隔离匹配文本的各个部分，进一步扩展这些功能来创建一个解析工具。通过将模式包围在小括号中（即 “(” 和 “)”）来分组。

from re_test_patterns import test_patterns

test_patterns(
	'abbaaabbbbaaaaa',
	[ ('a(ab)', 'a followed by literal ab'),
	  ('a(a*b*)', 'a followed by 0-n a and 0-n b'),
	  ('a(ab)*', 'a followed by 0-n ab'),
	  ('a(ab)+', 'a followed by 1-n ab'),
	  ])

    任何完整的正则表达式都可以转换为组，并嵌套在一个更大的表达式中。所有重复修饰符可以应用到整个组作为一个整体，这就要求重复整个组模式。
    要访问一个模式中单个组匹配的子串，可以使用 Match 对象的 groups() 方法。

import re

text = 'This is some text -- with punctuation.'

print text
print

patterns = [
    (r'^(\w+)', 'word at start of string'),
    (r'(\w+)\S*$', 'word at end, with optional punctuation'),
    (r'(\bt\w+)\W+(\w+)', 'word starting with t, another word'),
    (r'(\w+t)\b', 'word ending with t'),
    ]

for pattern, desc in patterns:
	regex = re.compile(pattern)
	match = regex.search(text)
	print 'Pattern %r (%s)\n' % (pattern, desc)
	print '  ', match.groups()
	print

    Match.groups() 会按表达式中与字符串匹配的组的顺序返回一个字符串序列。
    使用 group() 可以得到某个组的匹配，如果使用分组来查找字符串的各部分，不过结果中并不需要某些与组匹配的部分，此时 group() 会很有用。

import re

text = 'This is some text -- with punctuation.'

print 'Input text :', text

# word starting with 't' then another word
regex = re.compile(r'(\bt\w+)\W+(\w+)')
print 'Pattern                :', regex.pattern

match = regex.search(text)
print 'Entire match           :', match.group(0)
print 'Word starting with "t" :', match.group(1)
print 'Word after "t" word    :', match.group(2)

    第 0 组表示与整个表达式匹配的字符串，子组按其左小括号在表达式中出现的顺序从 1 开始标号。
    Python 对基本分组语法做了扩展，增加了命名组（named group）。通过使用名字来指示组，这样以后就可以更容易地修改模式，而不必同时修改使用了匹配结果的代码。要设置一个组的名字，可以使用以下语法：(?P<name>pattern)。

import re

text = 'This is some text -- with punctuation.'

print text
print

for pattern in [ r'(?P<first_word>\w+)',
                 r'(?P<last_word>\w+)\S*$',
                 r'(?P<t_word>\bt\w+)\W+(?P<other_word>\w+)',
                 r'(?P<ends_with_t>\w+t)\b',
                 ]:
    regex = re.compile(pattern)
    match = regex.search(text)
    print 'Matching "%s"' % pattern
    print '  ', match.groups()
    print '  ', match.groupdict()
    print

    使用 groupdict() 可以获取一个字典，它将组名映射到匹配的子串，groups() 返回的有序序列还包含命名模式。
    以下是更新后的 test_patterns()，它会显示与一个模式匹配的编号组和命名组，使后面的例子更容易理解。

# re_test_patterns_groups.py

import re

def test_patterns(text, patterns=[]):
	"""Given source text and a list of patterns, look for 
	matches for each pattern within the taxt and print
	them to stdout.
	"""

	# Look for each pattern in the text and print the results
	for pattern, desc in patterns:
		print 'Pattern %r (%s)\n' % (pattern, desc)
		print '  %r' % text
		for match in re.finditer(pattern, text):
			s = match.start()
			e = match.end()
			prefix = ' ' * (s)
			print '  %s%r%s ' % (prefix, text[s:e], ' '*(len(text)-e)),
			print match.groups()
			if match.groupdict():
				print '%s%s' % (' ' * (len(text)-s), match.groupdict())
		print
	return

    因为组本身也是一个完整的正则表达式，所以组可以嵌套在其他组中，够成更复杂的表达式。

from re_test_patterns_groups import test_patterns

test_patterns(
	'abbaabbba',
	[ (r'a((a*)(b*))', 'a followed by 0-n a and 0-n b'),
	 ])

    在这个例子中，组 (a*) 会匹配一个空串，所以 groups() 的返回值包括这个空串作为匹配值。
    组对于指定候选模式也很有用。可以使用管道符号 (|) 指示应该匹配某一个或另一个模式。不过，要仔细考虑管道符号的放置。下面这个例子中的第一个表达式会匹配一个 a 序列后面跟有一个完全由某一个字母 （a 或 b）构成的序列。第二个模式会匹配一个 a 后面跟有一个可能包含 a 或 b 的序列。这两个模式很相似，不过得到的匹配结果完全不同。

from re_test_patterns_groups import test_patterns

test_patterns(
	'abbaabbba',
	[ (r'a((a+)|(b+))', 'a then seq. of a or seq. of b'),
	  (r'a((a|b)+)', 'a then seq. of [ab]'),
	 ])

    如果候选组不匹配，但是整个模式确实匹配，groups() 的返回值会在序列中本应出现候选组的位置上包含一个 None 值。
    如果匹配子模式的字符串并不是从整个文本抽取的一部分，此时定义一个包含子模式的组也很有用。这些组称为“非捕获组”（noncapturing）。非捕获组可以用来描述重复模式或候选模式，而不在返回值中区分字符串的匹配部分。要创造一个非捕获组，可以使用语法 (?:pattern)。
    对于一个模式，尽管其捕获和非捕获形式会匹配相同的结果，但是会返回不同的组，下面来加以比较。

from re_test_patterns_groups import test_patterns

test_patterns(
	'abbaabbba',
	[ (r'a((a+)|(b+))', 'capturing form'),
	  (r'a((?:a+)|(?:b+))', 'noncapturing'),
	 ])

搜索选项
    利用选项标志可以改变匹配引擎处理表达式的方式。可以使用位或（OR）操作结合这些标志，然后传递至 compile()、search()、match() 以及其他接受匹配模式完成搜索的函数。
    1. 不区分大小写的匹配
    IGNORECASE 使模式的字面量字符和字符区间与大小写字符都匹配。

import re

text = 'This is some text -- with punctuation.'
pattern = r'\bT\w+'
with_case = re.compile(pattern)
without_case = re.compile(pattern, re.IGNORECASE)

print 'Text:\n  %r' % text
print 'Pattern:\n  %s' % pattern
print 'Case-sensitive:'
for match in with_case.findall(text):
	print '  %r' % match
print 'Case-insensitive:'
for match in without_case.findall(text):
	print '  %r' % match

    由于这个模式包含字面量字符 T，但没有设置 IGNORECASE，因此只有一个匹配，即单词 This。如果忽略大小写，那么 text 也能匹配。
    2. 多行输入
    有两个标志会影响如何在多行输入中进行搜索：MULTILINE 和 DOTALL。MULTILINE 标志会控制模式匹配代码如何对包含换行符的文本处理锁定指令。当打开多行模式时，除了整个字符串外，还要在每一行的开头和结尾应用 ^ 和 $ 的锚定规则。

import re

text = 'This is some text -- with punctuation.\nA second line.'
pattern = r'(^\w+)|(\w+\S*$)'
single_line = re.compile(pattern)
multiline = re.compile(pattern, re.MULTILINE)

print 'Text:\n  %r' % text
print 'Pattern:\n  %s' % pattern
print 'Single Line:'
for match in single_line.findall(text):
	print '  %r' % (match,)
print 'Multiline  :'
for match in multiline.findall(text):
	print '  %r' % (match,)

    这个例子中的模式会匹配输入的第一个或最后一个单词。它会匹配字符串末尾的 line，尽管并没有换行符。
    DOTALL 也是一个与多行文本有关的标志。正常情况下，点字符 （.）可以与输入文本中除了换行符之外的所有其他字符匹配。这个标志则允许点字符还可以匹配换行符。

import re

text = 'This is some text -- with punctuation.\nA second line.'
pattern = r'.+'
no_newlines = re.compile(pattern)
dotall = re.compile(pattern, re.DOTALL)

print 'Text:\n  %r' % text
print 'Pattern:\n  %s' % pattern
print 'No newlines:'
for match in no_newlines.findall(text):
	print '  %r' % match
print 'Dotall     :'
for match in dotall.findall(text):
	print '  %r' % match

    如果没有这个标志，输入文本的各行会与模式单独匹配。增加了这个标志后，则会利用整个字符串。
    3. Unicode
    在 Python 2 中，str 对象使用的是 ASCII 字符集，而且正则表达式处理会假设模式和输入文本都是 ASCII 字符。先前描述的转义码就默认使用 ASCII 来定义。要在 Python 2 中启用 Unicode 匹配，需要在编译模式时或者调用模块级函数 search() 和 match() 时增加 Unicode 标志。

#coding:utf-8
import re
import codecs
import sys

# Set standard output encoding to UTF-8
sys.stdout = codecs.getwriter('UTF-8')(sys.stdout)

text = u'Français zloty Österreich'
pattern = ur'\w+'
ascii_pattern = re.compile(pattern)
unicode_pattern = re.compile(pattern, re.UNICODE)

print 'Text:\n  %r' % text
print 'Pattern:\n  %s' % pattern
print 'ASCII  :', u', '.join(ascii_pattern.findall(text))
print 'Unicode:', u', '.join(unicode_pattern.findall(text))

    对于 Unicode 文本，其他转义序列（\W、\b、\B、\d、\D、\s 和 \S）也会做不同的处理。正则表达式引擎不再假设字符集成员由转义序列标识，而会查看 Unicode 数据库来查找各个字符的属性。
    注意：Python 3 对所有字符串都默认使用 Unicode，所以这个标志已经不再需要。
    4. 详细表达式语法
    随着表达式变得越来越复杂，紧凑格式的正则表达式语法可能会成为障碍。随着表达式中组数的增加，需要做更多的工作来明确为什么需要各个元素以及表达式的各部分究竟如何交互。使用命名组有助于缓解这些问题，不过一种更好的解决方案是使用详细模式表达式（verbose mode expression），它允许在模式中嵌入注释和额外的空白符。
    可以用一个验证 Email 地址的模式来说明采用详细模式能够更容易地处理正则表达式。第一个版本会识别以 3 个顶级域之一（.com、.org 和 .edu）结尾的地址。

import re

address = re.compile('[\w\d.+-]+@([\w\d.]+\.)+(com|org|edu)', re.UNICODE)

candidates = [
    u'first.last@example.com',
    u'first.last+category@gmail.com',
    u'valid-address@mail.example.com',
    u'not-valid@example.foo',
    ]

for candidate in candidates:
	match = address.search(candidate)
	print '%-30s  %s' % (candidate, 'Matches' if match else 'No match')

    这个表达式已经很复杂了，其中有多个字符类、组和重复表达式。将这个表达式转换为一种更详细的格式，使之更容易扩展。

import re

address = re.compile(
	'''
	[\w\d.+-]+        # username
	@
	([\w\d.]+\.)+     # domain name prefix
	(com|org|edu)     # TODO: support more top-level domains
	''', 
	re.UNICODE | re.VERBOSE)

candidates = [
    u'first.last@example.com',
    u'first.last+category@gmail.com',
    u'valid-address@mail.example.com',
    u'not-valid@example.foo',
    ]

for candidate in candidates:
	match = address.search(candidate)
	print '%-30s  %s' % (candidate, 'Matches' if match else 'No match')

    这个表达式会匹配同样的输入，不过采用这种扩展格式将更易读。注释还有助于标识模式中的不同部分，从而能扩展来匹配更多输入。
    这个扩展的版本会解析包含人名和 Email 地址的输入（这很可能在 Email 首部出现）。首先是单独的人名，然后是 Email 地址，用尖括号包围（“<” 和 “>”）。

import re

address = re.compile(
	'''

	# A name is made up of letters, and may include "."
	# for title abbreviations and middle initials.
	((?P<name>
	   ([\w.,]+\s+)*[\w.,]+)
       \s*
       # Email addresses are wrapped in angle
       # brackets: < > but only if a name is
       # found, so keep the start bracket in this
       # group.
       <
    )? # the entire name is optional

    # The address itself: username@domain.tld
    (?P<email>
      [\w\d.+-]+       # username
      @
      ([\w\d.]+\.)+    # domain name prefix
      (com|org|edu)    # TODO: support more top-level domains
    )

    >? # optional closing angle bracket
    ''',
    re.UNICODE | re.VERBOSE)

candidates = [
    u'first.last@example.com',
    u'first.last+category@gmail.com',
    u'valid-address@mail.example.com',
    u'not-valid@example.foo',
    u'First Last <first.last@example.com>',
    u'No Brackets first.last@example.com',
    u'First Last',
    u'First Middle Last <first.last@example.com>',
    u'First M. Last <first.last@example.com>',
    u'<first.last@example.com>',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Name :', match.groupdict()['name']
		print '  Email:', match.groupdict()['email']
	else:
		print '  No match'

    类似于其他编程语言，能够在详细正则表达式中插入注释将有助于增强其可维护性。最后这个版本包含为将来的维护人员提供的实现说明，另外还包括一些空白符将各个组分开，并突出其嵌套层次。
    5. 在模式中嵌入标志
    如果编译表达式时不能增加标志，如将模式作为参数传入一个将在以后编译该模式的库函数时，可以把标志嵌入到表达式字符串本身。例如，要启用不区分大小写的匹配，可以在表达式开头增加 (?i)。

import re

text = 'This is some text -- with punctuation.'
pattern = r'(?i)\bT\w+'
regex = re.compile(pattern)

print 'Text      :', text
print 'Pattern   :', pattern
print 'Matches   :', regex.findall(text)

    因为这些选项控制了如何计算或解析整个表达式，所以它们总要放在表达式最前面。
    以下列出了所有标志的缩写：
    IGNORECASE -> i
    MULTILINE -> m
    DOTALL -> s
    UNICODE -> u
    VERBOSE -> x
    可以把嵌入标志放在同一个组中结合使用。例如，(?imu) 会打开相应选项，支持多行 Unicode 字符串不区分大小写的匹配。
前向或后向
    很多情况下，仅当模式中另外某个部分也匹配时才匹配模式的某一部分，这可能很有用。例如，在 Email 解析表达式中，两个尖括号分别标志为可选。不过，实际上尖括号必须成对，只有当两个尖括号都出现或都不出现时表达式才能匹配。修改后的表达式使用了一个肯定前向（positive look-ahead）断言来匹配尖括号对。前向断言语法为 (?=pattern)

import re

address = re.compile(
	'''

	# A name is made up of letters, and may include "."
	# for title abbreviations and middle initials.
	((?P<name>
	   ([\w.,]+\s+)*[\w.,]+
	 )
     \s+
    ) # name is no longer optional

    # LOOKAHEAD
    # Email addresses are wrapped in angle brackets, but only
    # if they are both present or neither is.
    (?= (<.*>$)       # remainder wrapped in angle brackets
    	|
    	([^<].*[^>]$) # remainder *not* wrapped in angle brackets
     )

    <? # optional opening angle bracket

    # The address itself: username@domain.tld
    (?P<email>
      [\w\d.+-]+       # username
      @
      ([\w\d.]+\.)+    # domain name prefix
      (com|org|edu)    # TODO: support more top-level domains
    )

    >? # optional closing angle bracket
    ''',
    re.UNICODE | re.VERBOSE)

candidates = [
    u'First Last <first.last@example.com>',
    u'No Brackets first.last@example.com',
    u'Open Bracket <first.last@example.com',
    u'Close Bracket first.last@example.com>',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Name :', match.groupdict()['name']
		print '  Email:', match.groupdict()['email']
	else:
		print '  No match'

    这个版本的表达式中出现了很多重要的变化。首先，name 部分不再是可选的。这说明单独的地址将不能匹配，还能避免匹配那些格式不正确的“名/地址”组合。“name” 组后面的肯定前向规则断言字符串的余下部分要么包围在一对尖括号中，要么不存在不匹配的尖括号：也就是尖括号要么都出现，要么都不出现。这个前向规则表述为一个组，不过前向组的匹配并不利用任何输入文本。这个模式的其余部分会从前向匹配之后的位置取字符。
    否定前向（negative look-ahead）断言（(?!pattern)）要求模式不匹配当前位置后面的文本。例如，Email 识别模式可以修改为忽略自动系统常用的 noreply 邮件地址。

import re

address = re.compile(
	'''
	^
	# An address: username@domain.tld

	# Ignore noreply address
	(?!noreply@.*$)
	
    [\w\d.+-]+       # username
    @
    ([\w\d.]+\.)+    # domain name prefix
    (com|org|edu)    # limit the allowed top-level domains
    
    $
    ''',
    re.UNICODE | re.VERBOSE)

candidates = [
    u'first.last@example.com',
    u'noreply@example.com',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Match:', candidate[match.start():match.end()]
	else:
		print '  No match'

    以 noreply 开头的地址与这个模式不匹配，因为前向断言失败。
    不用前向检查 Email 地址 username 部分中的 noreply，还可以使用语法 (?<!pattern) 改写这个模式，写为在匹配 username 之后使用一个否定后向断言（negative look-behind assertion）。

import re

address = re.compile(
	'''
	^
	# An address: username@domain.tld

	[\w\d.+-]+       # username

	# Ignore noreply address
	(?<!noreply)
	
    @
    ([\w\d.]+\.)+    # domain name prefix
    (com|org|edu)    # limit the allowed top-level domains
    
    $
    ''',
    re.UNICODE | re.VERBOSE)

candidates = [
    u'first.last@example.com',
    u'noreply@example.com',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Match:', candidate[match.start():match.end()]
	else:
		print '  No match'

    后向与前向匹配的做法稍有不同，表达式必须使用一个定长模式。只要字符数固定（没有通配符或区间），后向匹配也与允许重复。
    可以借助语法 (?<=pattern) 用肯定后向（positive look-behind）断言查找符合某个模式的文本。例如，以下表达式可以查找 Twitter handles。

import re

twitter = re.compile(
	'''
	# A twitter handle: @username
	(?<=@)
	([\w\d_]+)       # username
    ''',
    re.UNICODE | re.VERBOSE)

text = '''This text includes two Twitter handles.
one for @ThePSF, and one for the author, @doughellmann.
'''

print text
for match in twitter.findall(text):
	print 'Handle:', match

    这个模式会匹配能构成一个 Twitter 句柄的字符序列，只要字符序列前面有一个 @。
自引用表达式
    匹配的值还可以用在表达式后面的部分中。例如，前面的 Email 例子可以更新为只匹配由人名和姓组成的地址，为此要包含这些组的反向引用（back-reference）。要达到这个目的，最容易的办法就是使用 \num 按 id 编号引用先前匹配的组。

import re

address = re.compile(
	r'''

	# The regular name
	(\w+)               # first name
	\s+
	(([\w.]+)\s+)?      # optional middle name or initial
	(\w+)               # last name

	\s+

	<

	# The address: first_name.last_name@domain.tld
	(?P<email>
	  \1                # first name
	  \.
	  \4                # last name
	  @
	  ([\w\d.]+\.)+     # domain name prefix
	  (com|org|edu)     # limit the allowed top-level domains
	)

    >
    ''',
    re.UNICODE | re.VERBOSE | re.IGNORECASE)

candidates = [
    u'First Last <first.last@example.com>',
    u'Different Name <first.last@example.com>',
    u'First Middle Last <first.last@example.com>',
    u'First M. Last <first.last@example.com>',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Match name:', match.group(1), match.group(4)
		print '  Match email:', match.group(5)
	else:
		print '  No match'

    尽管这个语法很简单，不过按照数字 id 创建反向引用有两个缺点。从实用角度讲，当表达式改变时，这些组就必须重新编号，每一个引用可能都需要更新。另一个缺点是，采用这种方法只能创建 99 个引用，因为如果 id 编号有 3 位，就会解释为一个八进制字符值而不是一个组引用。另一方面，如果一个表达式有超过 99 个组，问题就不只是无法引用表达式中的某些组那么简单了，还会产生一些更严重的维护问题。
    Python 的表达式解析器包括一个扩展，可以使用 (?P=name) 指示表达式中先前匹配的一个命名组的值。

import re

address = re.compile(
	'''

	# The regular name
	(?P<first_name>\w+)               # first name
	\s+
	(([\w.]+)\s+)?      # optional middle name or initial
	(?P<last_name>\w+)               # last name

	\s+

	<

	# The address: first_name.last_name@domain.tld
	(?P<email>
	  (?P=first_name)                # first name
	  \.
	  (?P=last_name)                # last name
	  @
	  ([\w\d.]+\.)+     # domain name prefix
	  (com|org|edu)     # limit the allowed top-level domains
	)

    >
    ''',
    re.UNICODE | re.VERBOSE | re.IGNORECASE)

candidates = [
    u'First Last <first.last@example.com>',
    u'Different Name <first.last@example.com>',
    u'First Middle Last <first.last@example.com>',
    u'First M. Last <first.last@example.com>',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Match name:', match.groupdict()['first_name'],
		print match.groupdict()['last_name']
		print '  Match email:', match.groupdict()['email']
	else:
		print '  No match'

    编译地址表达式时打开了 IGNORECASE 标志，因为尽管正确的名字通常首字母会大写，但 Email 地址往往不会大写首字母。
    在表达式中使用反向引用还有一种机制，即根据前一个组是否匹配来选择不同的模式。可以修正这个 Email 模式，使得如果出现名字就需要有尖括号，不过如果只有 Email 地址本身就不需要尖括号。查看一个组是否匹配的语法是 (?(id)yes-expression|no-expression)，这里 id 是组名或编号，yes-expression 是组有值时使用的模式，no-expression 则是组没有值时使用的模式。

import re

address = re.compile(
	'''

	# A name is made up of letters, and may include "."
	# for title abbreviations and middle initials.
	(?P<name>
	   ([\w.]+\s+)*[\w.]+
	 )?
	\s*

	# Email addresses are wrapped in angle brackets, but
	# only if a name is found.
	(?(name)
	  # remainder wrapped in angle brackets because
	  # there is a name
	  (?P<brackets>(?=(<.*>$)))
	  |
	  # remainder does not include angle brackets without name
	  (?=([^<].*[^>]$))
	 )

    # Only look for a bracket if the look-ahead assertion
    # found both of them.
    (?(brackets)<|\s*)

    # The address itself: username@domain.tld
    (?P<email>
      [\w\d.+-]+       # username
      @
      ([\w\d.]+\.)+    # domain name prefix
	  (com|org|edu)    # limit the allowed top-level domains
	 )
    
    # Only look for a bracket if the look-ahead assertion
    # found both of them.
    (?(brackets)>|\s*)

    $
    ''',
    re.UNICODE | re.VERBOSE)

candidates = [
    u'First Last <first.last@example.com>',
    u'No Brackets first.last@example.com',
    u'Open Bracket <first.last@example.com',
    u'Close Bracket first.last@example.com>',
    u'no.brackets@example.com',
    ]

for candidate in candidates:
	print 'Candidate:', candidate
	match = address.search(candidate)
	if match:
		print '  Match name:', match.groupdict()['name']
		print '  Match email:', match.groupdict()['email']
	else:
		print '  No match'

    这个版本的 Email 地址解析器使用了两个测试。如果 name 组匹配，则前向断言要求两个尖括号都出现，并建立 brackets 组。如果 name 组不匹配，这个断言则要求余下的文本不能用尖括号括起来。接下来，如果设置了 brackets 组，具体的模式匹配代码会借组字面量模式利用输入中的尖括号；否则，它会利用所有空格。
