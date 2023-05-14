# Отчет по лабораторной работе хеш-таблицы (МФТИ 2023)
____

## Цель работы
Исследование и сравнение разных хеш-функций, оптимизация алгоритма поиска данных в хеш-таблице.

## Введение
**Хеш-таблица** - это структура данных, реализующая удобную систему хранения пар ключ-значение. Она обычно реализуется в двух видах: массив пар ключ-значение или массив списков из этих элементов. В нашем случае будет разобран второй случай, но он будет упрощён отсутствием в паре ключ-значение самого значения, чтобы не усложнять код в учебных целях. В данной работе хеш-таблица представляет из себя массив из связных списков (их реализация представлена в репозитории [List](https://github.com/Art8mish/List)), хранящих слова. В нашей хеш-таблице реализованы такие функции, как добавление ключа, удаление и его поиск. Добавление элемента в хеш-таблицу производится с помощью _хеш-функции_.

**Хеш-функция** - функция, ставящая в соответствие некоторому элементу его индивидуальный идентификатор (обычно число), найденный с помощью математического алгоритма.

В нашем случае данными являются строки (слова). Хеш-функция каждому слову ставит в соответствие индивидуальное число, по которому происходит индексация в массиве списков (хеш-таблицы), и добавление в соответствующий список. Минус такой реализации заключается в том, что из-за коллизий (получение одинаковых идентификаторов для некоторых слов) на один индекс массива хеш-таблицы приходится несколько элементов (список), и поиск нужного среди них сильно дольше поиска индекса (алгоритм этого поиска мы и будем ускорять в следующей части работы).

Пример хеш-таблицы

<img src="/imgs/hsh_tbl.png" style="height: 320px;"/>


## Ход работы
## Часть 1. Исследование и сравнение хеш-функций

### Реализация
В данной работе мы будем сравнивать семь разных хеш-функций, а точнее анализировать построенные с их помощью хеш-таблицы. В частности, для определения качества хеш-функции мы будем определять её дисперсию (меру разброса случайной величины индекса в массиве). Хорошей хеш-функцией является та, у которой распределение количества элементов по всему массиву равномерное (в каждом списке примерно одинаковое количество элементов). 

Размер массива хеш-таблицы определяется средним количеством элементов приходящихся на один список (_load factor_ или _коэффициент заполнения хеш-таблицы_). Хорошей хеш-таблицей считается таблица с коэффициентом заполнения около 3-4. В данной работе в учебных целях будем использовать от 10 для наглядности распределений.

Данные будем брать из переведенного на английский язык произведения _Ф.М. Достоевского "Преступление и наказание"_, причём без повторений (около 10000 уникальных слов), а размер массива хеш-таблицы - 1009. Того коэффициент заполнения получается с хорошей точностью 9,9 слов на список.

### Хеш-функции

В данной работе будут исследованы семь разных хеш-функций, их подбор обусловлен целью показать разные по качеству функции для сравнения. Дальше будут приведены названия функций и разбор их алгоритма подсчета хеша с предположениями о возможных распределениях.

#### Константа

```C++
uint64_t hash_cnst(const char *word)
{
    return 1;
}
```

Данная функция всегда возвращает единицу, таким образом мы получим хеш-таблицу, которая будет состоять из 1008 пустых списков и одного из 10000 слов. График распределения такой функции будет выглядеть как прямая $x = 1$, его визуализацию приводить нет смысла. Данная хеш-функция нужна, чтобы показать самый плохой случай распределения.

#### ASCII код первого символа

```C++
uint64_t hash_symb(const char *word)
{
    uint64_t hash = *word;

    return hash;
}
```

Хеш-функция возвращает ASCII код первого символа. По первым предположениям она не сможет показать хорошего распределения, так как алфавит из строчных и прописных букв охватывает только 52 значения (а индексов всего 1009), ещё и присутствует сильная зависимость от лексики языка, в которой первая буква слова будет чаще всего одна и та же, а каких-то не будет вообще.

#### Длина слова

```C++
uint64_t hash_strlen(const char *word)
{
    uint64_t hash = strlen(word);

    return hash;
}
```

Хеш-функция возвращает длину слова. Она также как и предыдущая не покажет хорошего результата, так как длина английского слова в среднем варьируется от 5 до 15 (в любом случае не будет больше 30), и все слова будут распределены по первым 25-30 спискам.

#### Сумма ASCII кодов всех символов

```C++
uint64_t hash_ascii(const char *word)
{
    uint64_t hash = 0;
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
uint64_t hash_rol(const char *word)
{
    uint64_t hash = 0;
    while (*word != '\0')
    {
        hash = rol1(hash) ^ (*word);
        word++;
    }

    return hash;
}
```
Данная хеш-функция основана на ассемблерной команде `rol`, которая производит побитовый сдвиг влево с переносом старших разрядов на младшие (циклический сдфиг). Реализация сдвига на один бит реализован cледующим образом:

```C++
static uint64_t rol1(uint64_t num)
{
    uint64_t bit = num >> (sizeof(num) * 8 - 1);

    return (num << 1) | bit;
}
```

Предположительно эта хеш-функция должна показать хорошую статистику распределения за счет довольно случайного разброса по значениям, а также увеличения хеша в каждой итерации цикла как минимум в два раза.


#### Ror

```C++
uint64_t hash_ror(const char *word)
{
    uint64_t hash = 0;
    while (*word != '\0')
    {
        hash = ror1(hash) ^ (*word);
        word++;
    }

    return hash;
}
```

Хеш-функция ror отличается от предыдущей использованием команды `ror` вместо rol, циклический сдвиг производится вправо. Реализация сдвига на 1 бит почти идентична rol:

```C++
static uint64_t ror1(uint64_t num)
{
    uint64_t bit = num << (sizeof(num) * 8 - 1);

    return (num >> 1) | bit;
}
```

Сложно пока что-то сказать о результатах, которые данная функция покажет.

#### Addmul

```C++
static int HSH_CNST = 33;
uint64_t hash_addmul(const char *word)
{
    uint64_t hash = HSH_CNST;

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

Для того, чтобы сравнить хеш-функции, нужно ввести какую-то систему оценивания степени результативности функций. Идеальной функцией будет считаться та, которая распределит слова по спискам наиболее равномерно, то есть в каждом списке около десяти слов. Для этого будем рассматривать дисперсию случайной величины индекса в массиве хеш-таблицы. 

**Дисперсия** - мера разброса значений случайной величины относительно её математического ожидания (обозн. $D\xi$, где $\xi$ - случайная величина). 
**Математическое ожидание** - среднее значение случайной величины (обозн. $E\xi$). Проще говоря, дисперсия - это квадрат _стандартного отклонения_ случайной величины, которое характеризует степень разброса случайной величины относительно среднего значения или же математического ожидания (дальше будет использоваться сокращенный вариант названия _мат. ожидание_). Таким образом, мат. ожидание будет означать относительно индекса какого списка будет распределение ("хорошим" будет считаться значение около середины массива, то есть примерно 500). Соответственно, значение дисперсии чем больше, тем лучше (тем больше разброс и равномернее распределение слов по массиву). Для дисперсии приемлемым значением будет что-то в несколько десятков тысяч.

Считать мат. ожидание будем как среднее значение индекса массива для каждого слова, то есть сумма всех хешей для каждого слова, делённая на количество слов. Дисперсия определяется, как разность мат. ожидания квадрата случайной величины и квадрата её мат. ожидания: $D\xi = E(\xi)^{2} - (E\xi)^{2}$.

Получим таблицу с данными результатов распределения по всем хеш-функциям:

|  Хеш-функция  | cnst  | symb  | strlen | ascii |  rol  |  ror  | addmul |
|:-------------:|:------|-------|--------|-------|-------|-------|--------|
| Мат. ожидание |   1   |  104  |   7    |  596  |  476  |  77   |  504   |
|   Дисперсия   |   0   |  146  |   6    | 69791 | 74928 |  96   | 85791  |


Хеш-функция *hash_cnst* является худшей из всех возможных, исследовать её дальше не будем, так как с ней всё предельно понятно.

#### "Плохие" хеш-функции

<img src="/imgs/plot2.png" style="height: 320px;"/>

Сначала обсудим хеш-функции с плохим распределением (т.е. малой дисперсией), такими получились: *hash_symb*, *hash_ror*, *hash_strlen*. Как и ожидалось в случае с *hash_strlen* и *hash_symb*, они показали слишком плотные в одном месте распределения, оставляя большинство списков хеш-таблицы пустыми, за счёт небольшого диапазона значений длин слов и ASCII кодов первого символа.

Что же касается функции *hash_ror*, то результат не такой очевидный, но всё же объяснимый. Так как размер *uint64_t* - 8 байт (64 бита), и количество итераций цикла зависит от длины слова (~от 3 до 15), то старшие биты, полученные побитовом сдвигом не внесут вклад в ответ за счёт того, что итоговый индекс есть остаток от деления на размер массива (1009). И так как при каждой итерации _ror1_ идёт уменьшения хеша (деление на два без учёта старших разрядов), то итоговый диапазон значений получается довольно узким.

#### "Хорошие" хеш-функции

<img src="/imgs/plot1.png" style="height: 320px;"/>

Теперь разберём удачные хеш-функции, хорошее распределение показали: *hash_ascii*, *hash_rol*, *hash_addmul*. По графикам видно, что наиболее равномерное распределение получилось у последних двух.

Хеш-функция *hash_ascii* показала неплохое распределение, но количество слов в списках периодически то уменьшается, то увеличивается за счёт вклада не очень случайно распределённых параметров, как наиболее часто используемые буквы и длина слов.

Функции *hash_rol* и *hash_addmul* являются лучшими в сравнении с остальными, за счёт увелечения случайности результата. В каждой итерации цикла значение хеша довольно случайно растёт, и получающиеся числа охватывают весь диапазон индексов массива.

<img src="/imgs/plotrol.png" style="height: 320px;"/>

<img src="/imgs/plotaddmul.png" style="height: 320px;"/>

Стоит отметить, что значения мат. ожидания и дисперсии у хеш-функции *hash_addmul* ближе всего к требуемым.

> P.S. Все графики были построены с помощью `matplotlib`.

## Часть 2. Оптимизация алгоритма поиска ключа

### Цель и методика

Как понятно из заголовка, в данной части работы целью будет оптимизация алгоритма поиска ключа для конкретной хеш-функции. Возьмём функцию *hash_addmul*, так как у неё самые хорошие показатели разброса, это позволит получить более точные данные времени выполнения поиска.

Так как скорость выполнения одной итерации слишком мала для сравнения разных случаев, поместим функцию поиска в цикл с большим количеством итераций и будем измерять его время исполнения. Проводить будем по 10 измерений для каждого случая.

### Профилирование

Для того, что бы понять, что именно нужно оптимизировать, нужно определить, какие функции вносят наибольший вклад во время работы функции поиска.

**Профилирование** - сбор характеристик работы программы. В пределах данной работы понадобится время выполнения отдельных фрагментов и функций. Существует много инструментов для профилирования, подобные программы называются *профайлерами*. В данной работе для исследования времени работы функций был выбран фреймворк `valgrind` из-за наличия очень полезного плагина `callgrind`, который собирает данные по количеству вызовов и времени исполнения инструкций и формирует список функций, осуществляющих наибольший вклад во время исполнения программы. Перечень достоинств *callgrind* на этом не заканчивается, про остальные возможности данного плагина и, в принципе, фреймворка *valgrind* можно прочитать на официальном сайте https://valgrind.org/docs/manual/quick-start.html. Минус *callgrind* в том, что его время профилирования в несколько десятков раз превышает время исполнения программы, поэтому он не особо пригоден для анализа больших проектов, но для данной работы плагин подойдёт отлично.

Получим данные исполнения самых затратных по времени функций, чтобы определить, что в первую очередь следует оптимизировать. Ещё раз отмечу, что исследуемая функция поиска помещена в цикл с большим количеством итераций (10 000 000), чтобы исключить влияние остальных функций и получить удобный для сравнения по времени исполнения результат. Все оптимизации будут производиться с флагом `-O0`ы для наглядности в учебнных целях (на самом деле с флагами оптимизации `-O1` или `-O2` в плане написания оптимального ассемблерного кода соревноваться тяжело и для того, чтобы показать наибольшее количество оптимизаций, будем рассматривать результаты исполнения с флагом `-O2` в начальном состоянии как эталонные).

Приведём начальный код оптимизуремой функции:

```C++
int tblFindKey(HshTbl *hsh_tbl, void *word)
{
    ERR_CHCK(hsh_tbl == NULL, -1);
    ERR_CHCK(word    == NULL, -1);

    unsigned int hash = (unsigned int)hsh_tbl->hsh_fnc(word);
    hash = hash % hsh_tbl->size;

    List *lst = hsh_tbl->lst_arr[hash];
    ERR_CHCK(lst            == NULL, -1);
    ERR_CHCK(lst->fict_node == NULL, -1);

    Node *cur_nod = lst->fict_node->next;
    int wrd_flg = 0;

    for (unsigned i = 0; i < lst->size; i++)
    {
         if (strcmp((const char *)word, (const char *)cur_nod->value) == 0)
        {
            wrd_flg = 1;
            break;
        }

        cur_nod = cur_nod->next;
    }

    return wrd_flg;
}
```

Функция возвращает 1, если слово в таблице найдено, 0, если нет, и -1 при ошибке (лучшим вариантом будет для каждой ошибки возвращать индивидуальное значение, но чтобы не усложнять код, оставим -1 для всех ошибок). `ERR_CHCK` представляет собственный `assert`, он нужен для отладки (выходит из функции, возвращая код ошибки, и печатает место ошибки). Подобный *сухой* код приведён, чтобы показать, как важно при завершении стадии *дебага* отключать все лишние распечатки и ассерты.

Время выполнения в начальном состоянии:

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 0,510 | 0,257 | 0,267 |


Проведём же всё таки профилирование и получим начальные данные.

<img src="/imgs/callgrind0.png" style="height: 140px;"/>

Видно, что самый больший вклад у функции подсчёта хеша, поэтому начнём с неё.

### Оптимизация 1. Хеш-функция на ассемблере

Реализуем функцию *hash_addmul* на ассемблере. Константу *HASH_CNST* будем считать неизменно равной 33. Умножение на 33 в ассемблерном виде представлено побитовым смещением на пять разрядов влево (умножение на 32) и прибавление к старому значению.

```asm
_Z16_asm_hash_addmulPKc:

        push rbp                ;prologue
        mov rbp, rsp            ;

        mov rax, 33             ;HASH_CNST = 33
        jmp .lchck

.loop:  add rax, rdx            ;hash += *word

        mov rdx, rax            ;
        sal rdx, 5              ;
        add rax, rdx            ;hash *= 33

        inc rdi                 ;word++
        
.lchck: xor rdx, rdx
        mov byte dl, [rdi]
        cmp dl, 0x00            ;if (*word == '\0') break;
        jne .loop

.end:   pop rbp
        ret
```

Получим данные по времени исполнения:

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 0.360 | 0.265 | 0.264 |
|   Абс. ускорение    | 1.416 | 0.969 | 1.010 |
|   Отн. ускорение    | 1.416 | 0.969 | 1.010 |

Проведём профилирование.

<img src="/imgs/callgrind_asm_hash.png" style="height:140px;"/>

Влияние хеш-функции значительно снизилось. Программа с флагом -O0 ускорилась в 1.46 раз. Время исполнения с флагами -O1 и -O2 не изменилось, так как с ними создаётся и так оптимальный ассемблерный код для всех функций.

### Оптимизация 2. Strcmp на ассемблере

Внутри самой функции `tblFindKey` (без вызовов других функций) почти нечего оптимизировать, поэтому займёмся оптимизацией `strcmp`, так как она является следующей в списке. Результат прошлой оптимизации наталкивает на мысль снова написать её на ассемблере, но теперь осуществим ассемблерную вставку.

```C++
int inline asm_strcmp(const char* str1, const char* str2)
{
    int ret = 0;

    __asm__
    (
        ".intel_syntax noprefix \n\t"

        "mov rdi, %1            \n\t"
        "mov rsi, %2            \n\t"

        "xor eax, eax           \n\t"
        "xor ebx, ebx           \n\t"

        "_asm_strcmp_loop:      \n\t"
        "mov al, byte ptr [rdi] \n\t"
        "mov bl, byte ptr [rsi] \n\t"

        "cmp al, 0              \n\t"
        "je _asm_strcmp_end     \n\t"
        "cmp bl, 0              \n\t"
        "je _asm_strcmp_end     \n\t"

        "cmp al, bl             \n\t"
        "jne _asm_strcmp_end    \n\t"

        "inc rdi                \n\t"
        "inc rsi                \n\t"
        "jmp _asm_strcmp_loop   \n\t"

        "_asm_strcmp_end:       \n\t"
    	"sub eax, ebx           \n\t"
        "mov %0, eax\n\t"

        ".att_syntax prefix\n\t"

        : "=r" (ret) : "r" (str1), "r" (str2) 
        : "rax", "rbx", "rdi", "rsi", "r8", "r9"
    );

    return ret;
}
```

Ключевое слово `inline` означает подстановку ассемблерного кода, вместо вызова функции, что увеличивает количество кода, но немного ускоряет программу, потому что вызов функции тоже занимает время.

Результаты времени исполнения:

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 0.524 | 0.418 | 0.408 |
|   Абс. ускорение    | 0.975 | 0.615 | 0.654 |
|   Отн. ускорение    | 0.688 | 0.635 | 0.648 |

Из таблицы видно, что программа, как ни странно, замедлилась и относительное ускорение меньше единицы, поэтому нет особого смысла осуществлять профилирование. Функция `strcmp-avx2` оказалась быстрее, так как она основана на `SIMD` инструкциях, о которых можно прочитать на репозитории [SIMD](https://github.com/Art8mish/SIMD). Откатим эту оптимизацию и попробуем написать свою версию `avx_strcmp`.

### Оптимизация 3. SIMD

С помощью `SIMD` инструкций, предварительно представив все слова в виде векторов `__m256i`, реализуем функцию `avx_strcmp`.

```C++
int avx_strcmp(__m256i str1, __m256i str2)
{
    __m256i mask = _mm256_cmpeq_epi8(str1, str2); //compare 8-bit integers

    str1 = _mm256_andnot_si256(mask, str1);       //wrd1[i] = !mask[i] & wrd1[i])
    str2 = _mm256_andnot_si256(mask, str2);       //wrd1[i] = !mask[i] & wrd1[i])
    
    __m256i diff = _mm256_sub_epi8(str1, str2);

    for (int i = 0; i < 32; i++)
    {
        if (((int8_t *)&diff)[i])
            return ((int8_t *)&diff)[i];
    }

    return 0;
}
```

Замерим время исполнения:

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 1.437 | 0.528 | 0.515 |
|   Абс. ускорение    | 0.355 | 0.487 | 0.518 |
|   Отн. ускорение    | 0.251 | 0.503 | 0.513 |

Полученные данные ещё хуже предыдущих, исследуем функцию с помощью профилирования.


<img src="/imgs/avx_strcmp.png" style="height: 240px;"/>

Из результатов видно, что самым "трудным" яввляется цикл. Так как для данной задачи не требуется знать, какое слово больше, то можно заменить `strcmp` на *SIMD* функцию `int _mm256_testc_si256 (__m256i a, __m256i b)`, которая возвращает значение выражения (!a && b).

```C++
if (_mm256_testc_si256(wrd1, wrd2))
{
    wrd_flg = 1;
    break;
}
```

Теперь ещё раз проведём измерения времени исполнения.

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 0.351 | 0.190 | 0.190 |
|   Абс. ускорение    | 1.455 | 1.353 | 1.406 |
|   Отн. ускорение    | 1.027 | 1.397 | 1.393 |

Время исполнения уменьшилось (при чём для флагов оптимизации тоже), коэффициент относительного ускорения хоть и больше единицы, но уже сильно меньше предыдущего значения, поэтому можно переходить к заключительным оптимизациям.

### Заключительные оптимизации

В самом начале обсуждалось, что коэффициент заполнения хеш-таблицы в учебных целях около 10, поэтому, чтобы сделать хеш-таблицу оптимальной и сильно ускорить время поиска, увеличим размер массива списков до 3001 (чтобы сделать коэффициент около 3-4). Смысла увеличивать размер дальше особо нет, так как таблица будет занимать слишком большое количество памяти.

Полученные данные времени поиска:

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 0.321 | 0.174 | 0.175 |
|   Абс. ускорение    | 1.591 | 1.476 | 1.527 |
|   Отн. ускорение    | 1.094 | 1.091 | 1.086 |

Перед тем, как завершать данную часть работы, вспомним про ассерты для дебага, о которых я говорил в начале.

<img src="/imgs/err_chck.png" style="height: 240px;"/>

Профилирование показывает, что проверки вносят свой не самый малый вклад с учётом их количества. Проверим это: уберём проверки для финальной реализации и замерим время ещё раз.

|  Флаг оптимизации   |  -O0  |  -O1  |  -O2  |
|:-------------------:|:------|-------|------:|
| Время исполнения, с | 0.270 | 0.173 | 0.168 |
|   Абс. ускорение    | 1.893 | 1.481 | 1.590 |
|   Отн. ускорение    | 1.190 | 1.003 | 1.041 |

Из таблицы видно, что относительное ускорение в данном случае составило 1.190, что в больших проектах является довольно серьезным приростом производительности. Из этого можно сделать вывод, что нельзя забывать про отключение средств для дебага, они сильно замедляют исполнение программы (хорошим примером служит сам *valgrind*).

</b></details>
<details>   
<summary> Итоговые результаты профилирования </summary><br><b>

<img src="/imgs/callgrind_final.png" style="height: 140px;"/>

</b></details>

Все результаты профилирований можно посмотреть самому, в папке `.callgrind` хранятся все исходные файлы с данными. Для того что бы просмотреть информацию нужно прописать в консоль
`callgrind_annotate callgrind.out.NUM`, где NUM - индивидуальный номер файла.

| Стадия оптимизации |  Без оптимизаций  |  asm_hash  |  asm_strcmp  | avx_strcmp | SIMD | Итог |
|:------------------:|:-----|------|------|------|------|------|
| Номер файла | 2163 | 2447 | 3023 | 3472 | 3655 | 1829 |

## Вывод
В первой части данной работы мы познакомились с понятиями хеша, хеш-функции, хеш-таблицы. Исследовали и сравнили семь хеш-функций, определили критерии качества хеш-функции. Во второй части познакомились с профилированием и инструментом *valgrind* и его плагином *callgrind*. Были осуществлены три оптимизации: с помощью ассемблерной функции в отдельном *.asm* файле, с помощью ассемблерной вставки (была отброшена) и с использованием *SIMD* инструкций, а также приведение таблицы к оптимальным параметрам коэффициента заполнения. По результатам оптимизаций получено абсолютное ускорение в 1,893 без флагов оптимизации,  в 1,481 раз с флагом -O1 и в 1,590 раз с -O2.

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

4. Профилирование
    - https://ru.wikipedia.org/wiki/Профилирование_(информатика)
    - https://valgrind.org/docs/manual/quick-start.html (valgrind)
    - https://valgrind.org/docs/manual/cl-manual.html (callgrind)

4. Ссылки на связанные работы:
    - https://github.com/Art8mish/List
    - https://github.com/Art8mish/SIMD

5. Построение графиков (matplotlip)
    - https://www.geeksforgeeks.org/bar-plot-in-matplotlib/

Литература:
- Randal E. Bryant and David R. O'Hallaron "Computer Systems: A Programmer's Perspective"
