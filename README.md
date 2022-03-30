# getopt
Simple but powerful command-line options handler (C++11).

- 支持参数不存在时设置 default value
- support Chinese, 支持中文
- support multi opts，支持多关键词匹配，不限定关键词个数
- support spaces in args，支持参数中带空格。such as， the same result::
  -  -i C:\Program Files (x86)\Microsoft SDKs)
  -  -i "C:\Program Files (x86)\Microsoft SDKs)"
- auto add '-' if user forgot to add '-' when calling GetArg()/HasArg()



```c++
    GetOpt opts;

    auto num = opts.GetArg(1, "--n", "-num");
    auto input = opts.GetArg("", "-i", "--input", "--i", "-input");
    auto output = opts.GetArg("t:/", "--zh");
    bool pause = opts.HasArg("--p", "-p1");

```
windows support multiple space in args：
<pre>
getopt.exe --n 456 -i  "t:\a  b.txt" --zh a:\中文路径 带     空格  -p

--n   ->  456 
-i    ->  t:\a  b.txt 
--zh  ->  a:\中文路径 带     空格  
-p    ->  true
</pre>


linux（/proc/%d/cmdline） will squash to only one space if not surrounded by '"':

<pre>
getopt.exe --n 456 -i  "t:\a  b.txt" --zh a:\中文路径 带     空格  -p

--n   ->  456 
-i    ->  t:\a  b.txt 
--zh  ->  a:\中文路径 带 空格  
-p    ->  ture 
</pre>


```c++
    GetOpt opts_user("--n 456 -i  t:/a b.txt --zh a:/中文路径 带  空格  -p");

    auto u_num = opts_user.GetArg(1, "-n", "--num", "--n", "-num");
    auto u_input = opts_user.GetArg("", "-i", "--input", "--i", "-input");
    auto u_output = opts_user.GetArg("t:/", "--zh");
    bool u_pause = opts_user.HasArg("--p", "-p1");
```