% NIO23 Runde 2 Writeup
---
geometry:
  - a4paper
  - margin=2.5cm
classoption:
  - 12pt
lang: nb
header-includes:
  - \usepackage{setspace}
  - \usepackage{fancyhdr}
  - \usepackage{lastpage}
  - \usepackage{siunitx}
  - \usepackage{tikz}
  - \usetikzlibrary{arrows, positioning, automata}
  - \pagestyle{fancy}
  - \fancyhf{}
  - \onehalfspacing
  - \rfoot{Side \thepage\ av \pageref{LastPage}}
  - \renewcommand{\headrulewidth}{0pt}
colorlinks: true
linkcolor: black
urlcolor: blue
toc: true
numbersections: true
---

\thispagestyle{fancy}

# Lynnedslag

Jeg har to forskjellige løsninger for denne oppgaven, en som kjører i $\mathcal{O}(n + k \log n)$ tid og en som kjører i $\mathcal{O}(n + k)$ tid.

## Løsning ved segmenttrær

Problemet kan løses i $\mathcal{O}(n + k \log n)$ tid ved å representere tilstanden til hvert hus i et segmenttre av booleans, som implementerer disse operasjonene:

- `update(l, r)`: Toggler state til alle husene i range $[l, r)$
- `count()`: Teller antallet hus som fortsatt har strømforsyning. Denne operasjonen kan være implementert i $\mathcal{O}(n)$ tid siden den kun brukes en gang.

Pseudokode for problemet kan være:

```
Initialiser segmenttreet med (minst) n noder

For hvert lynnedslag:
    La l og r være index av det første og siste huset som blir truffet
    update(l, r + 1)

Print count()
```

## Lineær løsning {#1.2}

Problemet kan også løses i $\mathcal{O}(n + k)$. Representer tilstandene i en boolean array `a` hvor `a[i]` er `true` hvis hus `i` og hus `i - 1` har *forskjellig* tilstand (altså representerer `a` forskjellen i tilstand mellom konsekutive hus). Man kan toggle alle verdiene i rangen [l, r) ved å gjøre:

```
a[l] = !a[l];
a[r] = !a[r];
```

Til slutt går programmet gjennom `a` og rekonstruerer og skriver listen av tilstander slik oppgaven spør om.

# Bananbonanza

Bananbonanza er en standard DP-oppgave som kan løses i $\mathcal{O}(n)$ tid. Løsningen er gitt ved de rekursive formlene

\begin{align*}
f(x, c) &= -\infty \quad \text{hvis $x < 0$}\\
f(0, c) &= 0 \\
\\
f(x, \mathrm{K}) &= \max(f(x - 1, \mathrm{K}), f(x - 3, \mathrm{B}), f(x - 2, \mathrm{O}))\\
f(x, \mathrm{B}) &= \max(f(x - 1, \mathrm{B}), f(x - 3, \mathrm{K}) + b_x)\\
f(x, \mathrm{O}) &= \max(f(x - 1, \mathrm{O}), f(x - 2, \mathrm{K}) + o_x)
\end{align*}

Hvor $f(x, c)$ er den maksimale mengden du kan ha tjent etter $x$ dager i by $c$, K, B og O representerer Kristiansand, Bergen og Oslo og $b_x$ og $o_x$ er prisen i henholdsvis Bergen og Oslo ved dag $x$. Løsningen vil være den maksimale verdien av $f$ når $x = n - 1$.

# Personlighetstyper

(TODO)

# Nettverkssikkerhet

Problemet er en variant av maximal independent set problem[^1]. Det er en kant mellom alle par av noder som er direkte koblet til hverandre. En independent set i denne grafen består av en mengde noder (som *ikke* vil ha antivirus installert), og oppgaven går ut på å maksimere denne mengden (og derfor også minimere mengden av datamaskiner som trenger antivirus).

Det kan være nyttig å dele forklaringen opp etter subtask.

[^1]: "Maksimal antiklikk-problemet" eller "maksimal stabil mengde-problemet" på norsk tror jeg

## Subtask 1. $k = p = 0$ {#4.1}

Siden alle nodene vil ha grad $\le 2$ vil alle connected components i grafen enten være enkle stier eller sykler. I begge tilfeller kan man regne ut den maksimale independent set ved å kun vite antallet noder i grafen med formelen $\text{st\o rrelse} - \left\lfloor \frac{\text{st\o rrelse}}{2} \right\rfloor$ hvis komponenten er en enkel sti og $\text{st\o rrelse} - \left\lceil \frac{\text{st\o rrelse}}{2} \right\rceil$ hvis det er en sykel. Dette kan regnes ut over alle komponenter i $\mathcal{O}(n)$ tid.

## Subtask 2. $n = k$

Subtask 2 er NP-vanskelig hvis man fjerner begrensningen $K \le 15$, som kan vises via reduksjon fra idependent set problem. Siden $K$ er så liten er det mulig å løse denne med en $\mathcal{O}(k^2 2^k)$ bruteforce.[^2]

[^2]: Det finnes kanskje mer effektive løsninger men dette var raskt nok.

## Generell løsning

Ideen her er å ta hver independent set over de første $K$ nodene, for hver av disse finne størrelsen av maksimal independent set når man inkluderer resten av nodene, og bruke den største løsningen.

Man kan forenkle problemet litt: Del grafen inn i to deler, en del som består av noder som er (indirekte) koblet til en av de første nodene (heretter kalt $G_k$), og en del som består av resten av nodene ($G_l$). For den andre delen løser man grafen som i subtask 1. For å finne maximum independent set over resten av grafen kan det hjelpe å preprosessere grafen: Lag en ny graf, $H$, hvor nodene i $H$ er de første $k$ nodene i $G_k$ og hvor det er en kant mellom $x$ og $y$ hvis det finnes en direkte sti mellom $x$ og $y$ som ikke går gjennom noen annen av de første $k$ nodene i $G_k$. Kanten får vekt lik lengden av denne stien (ikke inkludert endepunktene). Preprosesseringen kan gjøres i $\mathcal{O}(n)$ tid. Merk at $H$ har $\mathcal{O}(k)$ kanter. Stier som ikke ender på en av de første $k$ nodene er representert som kanter mellom $x$ og en spesiell node kalt \num{-1}. \num{-1}-noden antas alltid å ha antivirus installert.

Når man vet lengden av en slik "sti" av noder, og man vet om endepunktene har antivirus eller ikke, går den an å regne ut i $\mathcal{O}(1)$ tid hvor mange noder på stien som ikke trenger antivirus, med formelen

\begin{equation*}
X = \max\left(0, \left\lceil \frac{\text{vekt} - \text{antall endepunkter som ikke har antivirus}}{2} \right\rceil\right)
\end{equation*}

Det vil si at hvis vi har en gyldig løsning for de første $k$ nodene kan vi regne ut den optimale løsningen for resten av nodene, hvis vi ikke tillater å endre på de første $k$ nodene. Vi kan prøve dette over alle independent sets i de første $k$ nodene, og velge den beste løsningen, med tidskompleksitet $\mathcal{O}(n + k^2 2^k)$.

# Godkjent

## Subtask 2. $G_i \le \num{100 000}$

Det er mulig å traversere hele treet for hver query[^3].

[^3]: Merk at i subtask 1 er $\sum{G_i} \le n m \le \num{100 000}$

## Subtask 3. $h \le 31$

Man kan representere antallet ganger hver ansatt har måttet godkjenne et arbeid på samme måte som i \ref{1.2}. Verdi `i` i arrayen vil representere `antall[i + 1] - antall[i]`, hvor `antall[i]` er antallet arbeid ansatt `i` har godkjent. Hvis de ansatte ordnes i post-order-rekkefølge kan alle oppdateringer gjøres med kun 4 array-operasjoner.

Algoritmen kan skrives i pseudokode som:

```
Initialiser listen av differanser med n noder (O(n))

For hvert arbeid (w = arbeider, g = antall godkjenninger som kreves),
  Hvis g = n, oppdater hele treet (O(log n))
  Ellers:
    Finn den øverste ascendenten a av w som har orden <= g (O(h))

    La al og ar være venstre og høyre endepunkt for
        den delen av listen som dekkes av a
    La p være den som har ansvar for a
    La pl være venstre endepunkt for den delen av
        listen som dekkes av p

    La g' = g - (ar - al)

    Hvis g' <= al - pl:
      Øk med 1 intervallet [al, ar)      (O(1))
      Øk med 1 intervallet [pl, pl + g') (O(1))
    Ellers:
      Øk med 1 intervallet [pl, pl + g)  (O(1))

Konverter listen av differanser til en liste av verdier s
ved å utføre en inclusive scan (O(n))

For hver node i treet, i rekkefølgen oppgitt i input,
  Skriv ut s[index til denne noden i listen]
```

Denne algoritmen har total tidskompleksitet $\mathcal{O}(n + k h)$.

## Generell løsning

Det som gjør at den forrige løsningen tar for mye tid på subtask 4 er at den gjør et lineært søk for node $a$ for hvert arbeid. Når $h \le 31$ er dette raskt nok men med ubegrenset $h$ er det for tregt. En ting man kan gjøre for å komme rundt dette er å lage "jump links" i nodene som lar deg hoppe over flere noder om gangen. Avhengig av hvilke noder som får slike pekere og hvor mange noder de hopper over vil dette gjøre det mulig å finne $a$ i logaritmisk tid.

Det finnes flere måter å velge hvilke noder som skal ha slike pekere og hvor mange noder de skal hoppe over, blant annet:

- Alle noder har en peker til noden $2^i$ noder ovenfor, for alle heltall i. Jeg har ikke testet dette men i teorien bør det kunne virke.
- Annenhver node har en peker til noden to noder ovenfor, hver fjerde node har en peker til noden fire noder ovenfor, osv. Dette er varianten jeg originalt gikk for.
- En regel beskrevet i <https://codeforces.com/blog/entry/100826>.

Den første og tredje vil la deg finne $a$ i $\mathcal{O}(\log h)$ tid, og den tredje krever kun $\mathcal{O}(n)$ tid for preprocessing, som gir en total tidskompleksitet på $\mathcal{O}(n + k \log h)$.

\begin{figure}[!htbp]
  \centering
  \begin{tikzpicture}
    \node[state] (T0) {$0$};
    \node[state] (T1) [right =of T0] {$1$};
    \node[state] (T2) [right =of T1] {$2$};
    \node[state] (T3) [right =of T2] {$3$};
    \node[state] (T4) [right =of T3] {$4$};
    \node[state] (T5) [right =of T4] {$5$};
    \node[state] (T6) [right =of T5] {$6$};
    \node[state] (T7) [right =of T6] {$7$};

    \tikzset{parentchild/.style={-, line width=1pt}};
    \path (T0) edge [parentchild] (T1);
    \path (T1) edge [parentchild] (T2);
    \path (T2) edge [parentchild] (T3);
    \path (T3) edge [parentchild] (T4);
    \path (T4) edge [parentchild] (T5);
    \path (T5) edge [parentchild] (T6);
    \path (T6) edge [parentchild] (T7);

    \tikzset{jump/.style={dashed, ->, line width=1pt}};
    \path (T1) edge [bend left, jump] (T0);
    \path (T2) edge [bend left, jump] (T1);
    \path (T3) edge [bend right, jump] (T0);
    \path (T4) edge [bend left, jump] (T3);
    \path (T5) edge [bend left, jump] (T4);
    \path (T6) edge [bend right, jump] (T3);
    \path (T7) edge [bend right, jump] (T0);
  \end{tikzpicture}
  \caption{En enkelt sti fra rotnoden i treet, hvor jumps er representert med stiplede linjer}
\end{figure}