# Introdução a Robotica

A porta computacional do robo é `COM10`.

## Introdução

Este relatório aborda a odometria e as equações de movimento de um robô móvel com tração diferencial. O robô utilizado possui duas rodas motrizes e uma roda livre para apoio. Através do uso de encoders, foram calculados parâmetros fundamentais para o controle do robô, como a constante de conversão de pulsos para metros e a distância entre rodas.

### Objetivo

O objetivo deste trabalho é implementar e validar a estimativa da pose do robô utilizando odometria e controlar a velocidade linear e angular das rodas por meio de um controlador PID+Feedforward. As principais etapas incluíram a medição da distância percorrida, o cálculo da distância entre rodas, e a implementação de um controle de linha para o robô seguir uma trajetória predefinida.

## CINEMÁTICA – TRAÇÃO DIFERENCIAL

A cinemática do robô é descrita pelas seguintes equações diferenciais, que relacionam as velocidades linear e angular às coordenadas e à orientação do robô:

```math
\frac{d}{dt}
\begin{pmatrix}
x(t) \\
y(t) \\
\theta(t)
\end{pmatrix}
=
\begin{pmatrix}
v(t) \cos(\theta(t)) \\
v(t) \sin(\theta(t)) \\
w(t)
\end{pmatrix}
```

A velocidade linear e angular do robô em função das velocidades das rodas $\text{v1(t) e v2(t)}$ são dadas por:

```math
\begin{aligned}
v(t) &= \frac{v_1(t) + v_2(t)}{2} \quad & w(t) &= \frac{v_1(t) + v_2(t)}{b} \\
d(i) &= \frac{d_1(i) + d_2(i)}{2} \quad & \Delta\theta(i) &= \frac{d_1(i) - d_2(i)}{b}
\end{aligned}
```

Onde 𝑏 é a distância entre as rodas motrizes.

## Medições Experimentais

### Constante que relaciona os impulsos com a distância percorrida

Para determinar a constante que relaciona os impulsos do encoder com a distância percorrida, foram realizados testes com diferentes valores de PWM nas rodas. A constante 𝑘 foi calculada da seguinte forma:

```math
k = \frac{\text{Distance Traveled (m)}}{\text{Impulses (Senc)}}
```

**Tabela 1**: Cálculo de 𝑘
| PWM v1 | PWM v2 | Senc1 | Senc2 | Δx1(m) | k1 = Δx1/Senc1 (m/impulse) | k2 = Δx2/Senc2 (m/impulse) | k~ |
| :----: | :----: | :---: | :---: | :----: | :------------------------: | :------------------------: | :-------: |
| 30 | 30 | 1663 | 1916 | 0.096 | 5.7727e-5 | 5.0104e-5 | 5.3916e-5 |
| 10 | 10 | 805 | 971 | 0.047 | 5.8385e-5 | 4.8403e-5 | 5.3394e-5 |
| 20 | 20 | 1924 | 2235 | 0.105 | 5.4574e-5 | 4.6980e-5 | 5.0777e-5 |

```math
k = 5.2696 \times 10^{-5} \, \text{(m/Senc)}
```

### Cálculo da distância entre as rodas

A distância entre as rodas 𝑏 foi calculada fazendo o robô girar 360° (2π radianos) com PWM aplicado em direções opostas nas rodas (valores negativos e positivos). A distância foi obtida utilizando a fórmula:

```math
 b= \frac{d_1(i) - d_2(i)}{\Delta\theta(i)}
```

**Tabela 2**: Cálculo de 𝑏
| PWM v1 | PWM v2 | Senc1 | Senc2 | d1 (m) | d2 (m) | Angl(rad) | b (m) |
| :----: | :----: | :---: | :---: | :-----: | :-----: | :-------: | :----: |
| 30 | -30 | 6899 | -7849 | 0.36354 | 0.41361 | $2\pi$ | 0.1196 |
| 10 | -10 | 6432 | -7831 | 0.33894 | 0.41266 | $2\pi$ | 0.1241 |
| 20 | -20 | 6839 | -7964 | 0.36038 | 0.41967 | $2\pi$ | 0.1236 |

```math
b = 0.1225 \, \text{(m)}
```

## Controle PID+Feedforward

### Feedforward

O ganho feedforward $K_f$ foi determinado com base na relação entre o PWM aplicado e a velocidade linear do robô em m/s. O valor médio obtido foi:

```math
\text{enc} = 100 \\ \text{pwm} = 50 \\
```

```math
v = \frac{k \times \text{enc}}{0.040} = 0.144914 \, \text{m/s}
```

```math
k_f = \frac{\text{pwm}}{v} = 345 \, \text{(m)}
```

### Controlador PID

O controlador PID utilizado possui a seguinte estrutura:

```math
\text{Out\_PID} = K_p \times \text{Erro} + K_i \times \text{Integral\_do\_erro} + K_d \times \text{Derivada\_do\_erro}
```

Os valores experimentais de $K_p$, $K_i$ e $K_d$ foram ajustados da seguinte forma:

1. Iniciou-se apenas com o ganho de feedforward $K_f$.
2. A velocidade de referência foi ajustada até que uma diferença mínima de velocidade entre as rodas fosse observada.
3. $K_p$ foi incrementado até que oscilações significativas ocorressem, reduzindo o valor final para 1/4 do valor obtido.
4. $K_i$ foi ajustado para eliminar erros de regime permanente, observando a estabilidade do sistema.

## Controle de Trajetória e Seguimento de Linha

Um controlador foi implementado para seguir uma linha utilizando os sensores IR do robô. Inicialmente, apenas os sensores das extremidades foram utilizados, e depois, o sensor central foi incorporado para seguir a borda da linha.

Procedimento para o Seguimento de Linha:
- Definiu-se um estado para seguir a linha (Estado 100), aumentando progressivamente a velocidade linear até o robô alcançar o limite de estabilidade.
- A transição entre diferentes partes da pista foi realizada via estados programados no arquivo `control.cpp`.


## Conclusão
Este relatório apresentou a implementação e validação de um sistema de odometria e controle de um robô móvel com tração diferencial. A constante de conversão de pulsos para metros e a distância entre rodas foram calculadas experimentalmente, e o controlador PID+Feedforward foi ajustado para otimizar o desempenho do robô.
