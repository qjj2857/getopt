# getopt
Simple but powerful command-line options handler (C++11).

- ֧�ֲ���������ʱ���� default value
- support Chinese, ֧������
- support multi opts��֧�ֶ�ؼ���ƥ�䣬���޶��ؼ��ʸ���
- support spaces in args��֧�ֲ����д��ո�such as�� the same result::
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
windows support multiple space in args��
<pre>
getopt.exe --n 456 -i  "t:\a  b.txt" --zh a:\����·�� ��     �ո�  -p

--n   ->  456 
-i    ->  t:\a  b.txt 
--zh  ->  a:\����·�� ��     �ո�  
-p    ->  true
</pre>


linux��/proc/%d/cmdline�� will squash to only one space if not surrounded by '"':

<pre>
getopt.exe --n 456 -i  "t:\a  b.txt" --zh a:\����·�� ��     �ո�  -p

--n   ->  456 
-i    ->  t:\a  b.txt 
--zh  ->  a:\����·�� �� �ո�  
-p    ->  ture 
</pre>


```c++
    GetOpt opts_user("--n 456 -i  t:/a b.txt --zh a:/����·�� ��  �ո�  -p");

    auto u_num = opts_user.GetArg(1, "-n", "--num", "--n", "-num");
    auto u_input = opts_user.GetArg("", "-i", "--input", "--i", "-input");
    auto u_output = opts_user.GetArg("t:/", "--zh");
    bool u_pause = opts_user.HasArg("--p", "-p1");
```