# Отчет по лабораторной работе хеш-таблицы (МФТИ 2023)
____

## Цель работы
Исследование и сравнение разных хеш-функций, оптимизация алгоритма поиска данных в хеш-таблице.

## Введение
**Хеш-таблица** - это структура данных, реализующая удобную систему хранения пар ключ-значение. Она обычно реализуется в двух видах: массив пар ключ-значение или массив списков из этих элементов. В нашем случае будет разобран второй случай, но он будет упрощён отсутвием в паре ключ-значение самого значения, чтобы не усложнять код в учебных целях. В данной работе хеш-таблица представляет из себя массив из связных списков (их реализация представлена в репозитории ![List](https://github.com/Art8mish/List)), хранящих слова. В нашей хеш-таблице реализованы такие функции, как добавление ключа, удаление и его поиск. Добавление элемента в хеш-таблицу производится с помощью _хеш-функции_.

**Хеш-функция** - функция, ставящая в соответствие некоторому элементу его индивидуальный идентификатор (обычно число), найденный с помощью математического алгоритма. Хеш-функция позволяет ускорить работу с неудобными данными.

Например, в нашем случае неудобными данными являются строки (слова). Хеш-функция каждому слову ставит в соответствие индивидуальное число, по которому происходит индексация в массиве списков (хеш-таблицы), и добавление в соответствующий список. Минус такой реализации заключается в том, что из-за коллизий (получение одинаковых идентификаторов для некоторых слов) на один индекс массива хеш-таблицы приходится несколько элементов (список), и поиск нужного среди них сильно дольше поиска индекса (алгоритм этого поиска мы и будем ускорять в следующей части работы).

Пример хеш-таблицы

<img src="/imgs/hsh_tbl.png" style="height: 320px;"/>


## Ход работы
## Часть 1. Исследование и сравнение хеш-функций

### Реализация
О том, что из себя представляет хеш-функция вы можете прочитать в введении. Что касается исследования хеш-функций в данной работе, мы будем сравнивать 7 разных хеш-функций, а точнее анализировать построенные с их помощью хеш-таблицы. В частности, для определения качества хеш-функции мы будем определять её дисперсию (меру разброса случайной величины индекса в массиве). Хорошей хеш-функцией является та, у которой распределение количества элементов по всему массиву равномерное (в каждом списке примерно одинаковое количество элементов). 

Размер массива хеш-таблицы определяется средним количеством элементов приходящихся на один список (_load factor_ или _коэффициент заполнения хеш-таблицы_). Хорошей хеш-таблицей считается таблица с коэффициентом заполнения около 3-4. В данной работе в учебных целях будем использовать от 10 для наглядности распределений.

Данные будем брать из переведенного на английский язык произведения _Ф.М. Достоевского "Преступление и наказание"_, причём без повторений (около 10000 уникальных слов), а размер массива хеш-таблицы - 1009. Того коэффициент заполнения получается с хорошей точностью 9,9 слов на список.

### Хеш-функции

В данной работе будут исследованы 7 разных хеш-функций, их подбор обусловлен целью показать разные по качеству функции для сравнения. Дальше будут приведены названия функций и разбор их алгоритма подсчета хеша.

> ull - сокращение от unsigned long long

#### Константа

```C++
ull hash_cnst(const char *word)
{
    return 1;
}
```

Данная функция всегда возвращает '1', таким образом мы получим хеш-таблицу, которая будет состоять из 1008 пустых списков и одного из 10000 слов. График распределения такой функции будет выглядеть как прямая '$x = 1$', его визуализацию приводить нет смысла. Данная хеш-функция нужна, чтобы показать самый плохой случай распределения.

#### ASCII код первого символа

```C++
ull hash_symb(const char *word)
{
    ull hash = *word;

    return hash;
}
```

Хеш-функция возвращает ASCII код первого символа. По первым предположениям она не сможет показать хорошего распределения, так как алфавит из строчных и прописных букв охватывает только 52 значения (а индексов всего 1009), ещё и присутствует сильная зависимость от лексики языка, в которой первая буква слова будет чаще всего одна и та же, а каких-то не будет вообще.

#### Длина слова

```C++
ull hash_strlen(const char *word)
{
    ull hash = strlen(word);

    return hash;
}
```

Хеш-функция возвращает длину слова. Она также как и предыдущая не покажет хорошего результата, так как длина английского слова в среднем варьируется от 5 до 15 (в любом случае не будет больше 30), и все слова будут распределены по первым 25-30 спискам.

#### Сумма ASCII кодов всех символов

```C++
ull hash_ascii(const char *word)
{
    ull hash = 0;
    while (*word != '\0')
    {
        hash += *word;
        word++;
    }

    return hash;
}
```

Данная хеш-функция возвращает сумму ASCII кодов слова. В сравнение с функцией, возвращающей ASCII код только первого символа, эта будет получше, потому что покрывает весь диапазон индексов, но по причинам зависимости от наиболее часто используемых букв всё равно предположительно покажет не такой хороший результат.

#### Rol

```C++
ull hash_rol(const char *word)
{
    ull hash = 0;
    while (*word != '\0')
    {
        hash = rol1(hash) ^ (*word);
        word++;
    }

    return hash;
}
```
Данная хеш-функция основана на ассемблерной команде `rol`, которая производит побитовый сдвиг влево с переносом старших разрядов на младшие (что-то вроде битового вращения). Реализация сдвига на 1 бит реализован cледующим образом:

```C++
static ull rol1(ull num)
{
    ull bit = num >> (sizeof(num) * 8 - 1);

    return (num << 1) | bit;
}
```

Предположительно эта хеш-функция должна показать хорошую статистику распределения за счет довольно случайного разброса по значениям, а также увеличения хеша в каждой итерации цикла как минимум в два раза.


#### Ror

```C++
ull hash_ror(const char *word)
{
    ull hash = 0;
    while (*word != '\0')
    {
        hash = ror1(hash) ^ (*word);
        word++;
    }

    return hash;
}
```

Хеш-функция ror отличается от предыдущей использованием команды `ror` вместо rol, побитовый сдвиг производится вправо. Реализация сдвига на 1 бит почти идентична rol:

```C++
static ull ror1(ull num)
{
    ull bit = num << (sizeof(num) * 8 - 1);

    return (num >> 1) | bit;
}
```

Сложно пока что-то сказать о результатах, которые данная функция покажет.

#### Addmul

```C++
static int HSH_CNST = 32;
ull hash_addmul(const char *word)
{
    ull hash = HSH_CNST;

    while (*word != '\0')
    {
        hash += *word;
        hash *= HSH_CNST;
        word++;
    }

    return hash;
}
```

Данная хеш-функция считает хеш по следующему алгоритму: начальное значение - заданная константа, к текущему значению хеша прибавляется ASCII код символа слова, затем значение хеша умножается на эту константу. Чтобы получалось довольно случайное число, которое охватывает большой диапазон индексов массива хеш-таблицы значение константы возьмём не меньше 30, пусть 33.

Для этой функции выбрано простое название addmul, которое отражает принцип алгоритма (так вообще делать не следует, но для простоты ассоциаций и учебных целей мы пойдём на это).

### Эксперимент

Получим данные распределения слов по спискам хеш-таблицы для каждой хеш-функции (кроме первой) и на основе этих данных построим столбчатые диаграммы: индекс списка в массиве по оси X и количество слов в списке по оси Y. 

В общем случае диаграмма выглядит слелующим образом:

<img src="/imgs/plotall.png" style="height: 320px;"/>

По данной диаграмме можно понять, как хеш-функции разделить по группам для сравнения. Сразу видно, что функции `hash_strlen`, `hash_symb`, `hash_ror` показывают плохое рапределение, в отличие от остальных.

Чтобы сравнить полученные распределения объединим некоторые диаграммы с похожими значениями по Y.

#### Принципы сравнения хеш-функций. Математическое ожидание и дисперсия.

Для того, чтобы сравнить хеш-функции, нужно ввести какую-то систему оценивания степени результативности функций. Идеальной функцией будет считаться та, которая распределит слова по спискам наиболее равномерно, то есть в каждом списке около 10 слов. Для этого будем рассматривать дисперсию случайной величины индекса в массиве хеш-таблицы. 

**Дисперсия** - мера разброса значений случайной величины относительно её математического ожидания (обозн. $D/xi$, где $/xi$ - случайная величина). **Математическое ожидание** - среднее значение случайной величины (обозн. $E/xi$). Проще говоря, дисперсия - это квадрат _стандартного отклонения_ случайной величины, которое характеризует степень разброса случайной величины относительно среднего значения или же математического ожидания (дальше будет использоваться сокращенный вариант названия _мат. ожидание_). Таким образом, мат. ожидание будет означать относительно индекса какого списка будет распределение ("хорошим" будет считаться значение около середины массива, то есть примерно 500). Соответственно, значение дисперсии чем больше, тем лучше (тем больше разброс и равномернее распределение слов по массиву). Для дисперсии приемлемым значением будет что-то в несколько десятков тысяч.

Считать мат. ожидание будем как среднее значение индекса массива для каждого слова, то есть сумма всех хешей для каждого слова, делённая на количество слов. Дисперсия определяется, как разность мат. ожидания квадрата случайной величины и квадрата её мат. ожидания: $D/xi = E(/xi)^{2} - (E/xi)^{2}$.

Получим таблицу с данными результатов распределения по всем хеш-функциям:

|  Хеш-функция  | cnst  | symb  | strlen | ascii |  rol  |  ror  | addmul |
|:-------------:|:------|-------|--------|-------|-------|-------|--------|
| Мат. ожидание |   1   |  104  |   7    |  596  |  476  |  77   |  504   |
|   Дисперсия   |   0   |  146  |   6    | 69791 | 74928 |  96   | 85791  |


Хеш-функция *hash_cnst* является худшей из всех возможных, исследовать её дальше не будем, так как с ней всё предельно понятно.

#### "Плохие" хеш-функции

<img src="/imgs/plot2.png" style="height: 320px;"/>

Сначала обсудим хеш-функции с плохим распределением (т.е. малой дисперсией), такими получились: *hash_symb*, *hash_ror*, *hash_strlen*. Как и ожидалось в случае с *hash_strlen* и *hash_symb*, они показали слишком плотные в одном месте распределения, оставляя большинство списков хеш-таблицы пустыми, за счёт небольшого диапазона значений длин слов и ASCII кодов первого символа.

Что же касается функции *hash_ror*, то результат не такой очевидный, но всё же объяснимый. Так как размер *unsigned long long* - 8 байт (64 бита), и количество итераций цикла зависит от длины слова (~от 3 до 15), то старшие биты, полученные побитовом сдвигом не внесут вклад в ответ за счёт того, что итоговый индекс есть остаток от деления на размер массива (1009). И так как при каждой итерации _ror1_ идёт уменьшения хеша (деление на 2 без учёта старших разрядов), то итоговый диапазон значений получается довольно узким.

#### "Хорошие" хеш-функции

<img src="/imgs/plot1.png" style="height: 320px;"/>

Теперь разберём удачные хеш-функции, хорошее распределение показали: *hash_ascii*, *hash_rol*, *hash_addmul*. По графикам видно, что наиболее равномерное распределение получилось у последних двух.

Хеш-функция *hash_ascii* показала неплохое распределение, но количество слов в списках периодически то уменьшается, то увеличивается за счёт вклада не очень случайно распределённых параметров, как наиболее часто используемые буквы и длина слов.

Функции *hash_rol* и *hash_addmul* являются лучшими в сравнении с остальными, за счёт увелечения случайности результата. В каждой итерации цикла значение хеша довольно случайно растёт, и получающиеся числа охватывают весь диапазон индексов массива.

<img src="/imgs/plotrol.png" style="height: 320px;"/>

<img src="/imgs/plotaddmul.png" style="height: 320px;"/>

Стоит отметить, что значения мат. ожидания и дисперсии у хеш-функции *hash_addmul* ближе всего к требуемым.

#### свёртка 

</b></details>
<details>   
<summary> текст </summary><br><b>

</b></details>

##### картинка
<img src="/pics/asm.png" style="height: 150px;"/>


## Вывод


## Источники и литература
1. Хеш-таблица:
    - https://ru.wikipedia.org/wiki/Хеш-таблица

2. Хеш-функция:
    - https://ru.wikipedia.org/wiki/Хеш-функция

3. Математическое ожидание и дисперсия:
    - https://ru.wikipedia.org/wiki/Математическое_ожидание
    - https://ru.wikipedia.org/wiki/Дисперсия_случайной_величины
    - https://studfile.net/preview/2481882/page:7/
    - https://www.codecamp.ru/blog/a-simple-explanation-of-how-to-interpret-variance/


Литература:
- Randal E. Bryant and David R. O'Hallaron "Computer Systems: A Programmer's Perspective"
