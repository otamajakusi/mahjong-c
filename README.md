![test](https://github.com/otamajakusi/mahjong-c/actions/workflows/test.yaml/badge.svg)

# mahjong-c

This repository provides a mahjong agari detection and score calculation library written in C.
Written in C, it is relatively fast, portable and has no library dependencies, not even malloc!

## Mahjong Rules

The library conforms to the [M-League rules](https://m-league.jp/about/).

The following is a list of the different interpretations depending on the rules.
- 連風牌の対子の符: 2符(M-League compliant)
- 嶺上牌のツモ符: 2符(M-League compliant)
- 緑一色の發: なくてもよい(M-League compliant)
- その他国士無双13面待ちや四暗刻単騎待ちをダブル役満とするローカルルールはMリーグを含め公式なルールでは採用されていません

See [src/yaku.h](src/yaku.h) for more details.

## Examples

```bash
$ make
$ make example
han: 4, fu: 40, yaku: sanshoku tanyao iipeiko
```

example.cのアガリ形は「平和, 断么九, 一盃口」とも解釈できますが、高点法により「三色同順, 断么九, 一盃口」を出力します。

## Licence

[MIT](LICENSE)
