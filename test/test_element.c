#include <stdio.h>
#include <assert.h>

#include "util.h"
#include "test_element.h"
#include "test_util.h"

void test_gen_elements_from_melds() {
    /* 1 */
    MJMelds melds1 = {
        {
            {{m3,m1,m2}, 3, false, -1},
            {{p2,p3,p1}, 3, false, -1},
            {{dr,dr,dr}, 3, false, -1},
            {{wt,wt,wt,wt}, 4, true, -1},
        },
        4
    };
    Elements act;
    Elements exp1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p1,p2,p3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{dr,dr,dr}, 3, false, ELEM_TYPE_TRIPLETS},
            {{wt,wt,wt,wt}, 4, true, ELEM_TYPE_FOURS},
        },
        4
    };
    memset(&act, -1, sizeof(Elements));

    assert(gen_elements_from_melds(&act, &melds1));
    assert(memcmp(&act, &exp1, sizeof(Elements)) == 0);

    MJMelds melds2 = {
        {
            {{m3,m3,m2}, 3, false, -1}, // neither sequence nor triplets
            {{p2,p3,p1}, 3, false, -1},
            {{dr,dr,dr}, 3, false, -1},
            {{wt,wt,wt,wt}, 4, true, -1},
        },
        4
    };
    assert(!gen_elements_from_melds(&act, &melds2));

    MJMelds melds3 = {
        {
            {{m3,m1,-1}, 3, false, -1}, // illegal tile id
            {{p2,p3,p1}, 3, false, -1},
            {{dr,dr,dr}, 3, false, -1},
            {{wt,wt,wt,wt}, 4, true, -1},
        },
        4
    };
    assert(!gen_elements_from_melds(&act, &melds3));
}

/* single element */
/* 2..8 */
void test_is_element_chunchan() {
    Element elem1 = {{m2,m3,m4}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem2 = {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem3 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem4 = {{m7,m8,m9}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem5 = {{wn,wn,wn}, 3, false, ELEM_TYPE_TRIPLETS};
    assert(is_element_chunchan(&elem1));
    assert(is_element_chunchan(&elem2));
    assert(!is_element_chunchan(&elem3));
    assert(!is_element_chunchan(&elem4));
    assert(!is_element_chunchan(&elem5));
}

/* 1 or 9 */
void test_is_element_routou() {
    Element elem1 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem2 = {{p9,p9,p9,p9}, 4, false, ELEM_TYPE_FOURS};
    Element elem3 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem4 = {{m7,m8,m9}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem5 = {{wn,wn,wn}, 3, false, ELEM_TYPE_TRIPLETS};
    assert(is_element_routou(&elem1));
    assert(is_element_routou(&elem2));
    assert(!is_element_routou(&elem3));
    assert(!is_element_routou(&elem4));
    assert(!is_element_routou(&elem5));
}

/* 123, 789, 111, 999 */
void test_has_element_routou() {
    Element elem1 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem2 = {{p9,p9,p9,p9}, 4, false, ELEM_TYPE_FOURS};
    Element elem3 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem4 = {{m7,m8,m9}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem5 = {{wn,wn,wn}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem6 = {{s3,s3,s3}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem7 = {{s3,s4,s5}, 3, false, ELEM_TYPE_SEQUENCE};
    assert(has_element_routou(&elem1));
    assert(has_element_routou(&elem2));
    assert(has_element_routou(&elem3));
    assert(has_element_routou(&elem4));
    assert(!has_element_routou(&elem5));
    assert(!has_element_routou(&elem6));
    assert(!has_element_routou(&elem7));
}

/* 1,9,字牌 */
void test_is_element_yaochu() {
    Element elem1 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem2 = {{p9,p9,p9,p9}, 4, false, ELEM_TYPE_FOURS};
    Element elem3 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem4 = {{m7,m8,m9}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem5 = {{wn,wn,wn}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem6 = {{s3,s3,s3}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem7 = {{s3,s4,s5}, 3, false, ELEM_TYPE_SEQUENCE};
    assert(is_element_yaochu(&elem1));
    assert(is_element_yaochu(&elem2));
    assert(!is_element_yaochu(&elem3));
    assert(!is_element_yaochu(&elem4));
    assert(is_element_yaochu(&elem5));
    assert(!is_element_yaochu(&elem6));
    assert(!is_element_yaochu(&elem7));
}

/* 123, 789, 111, 999 or 字牌 */
void test_has_element_yaochu() {
    Element elem1 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem2 = {{p9,p9,p9,p9}, 4, false, ELEM_TYPE_FOURS};
    Element elem3 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem4 = {{m7,m8,m9}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem5 = {{wn,wn,wn}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem6 = {{s3,s3,s3}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem7 = {{s3,s4,s5}, 3, false, ELEM_TYPE_SEQUENCE};
    assert(has_element_yaochu(&elem1));
    assert(has_element_yaochu(&elem2));
    assert(has_element_yaochu(&elem3));
    assert(has_element_yaochu(&elem4));
    assert(has_element_yaochu(&elem5));
    assert(!has_element_yaochu(&elem6));
    assert(!has_element_yaochu(&elem7));
}

void test_is_element_sequence() {
    Element elem1 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    Element elem2 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    assert(is_element_sequence(&elem1));
    assert(!is_element_sequence(&elem2));
}

void test_is_element_triplets() {
    Element elem1 = {{m5,m5,m5}, 3, false, ELEM_TYPE_TRIPLETS};
    Element elem2 = {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE};
    assert(is_element_triplets(&elem1));
    assert(!is_element_triplets(&elem2));
}

void test_is_element_fours() {
    Element elem1 = {{m4,m4,m4,m4}, 4, false, ELEM_TYPE_FOURS};
    Element elem2 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    assert(is_element_fours(&elem1));
    assert(!is_element_fours(&elem2));
}

void test_is_element_concealed() {
    Element elem1 = {{m4,m4,m4,m4}, 4, true, ELEM_TYPE_FOURS};
    Element elem2 = {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS};
    assert(is_element_concealed(&elem1));
    assert(!is_element_concealed(&elem2));
}

/* all elements */
void test_is_elements_chunchan() {
    Elements elems1 = {
        {
            {{m2,m3,m4}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p2,p3,p4}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        2
    };
    Elements elems2 = {
        {
            {{m2,m3,m4}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p1,p2,p3}, 3, false, ELEM_TYPE_SEQUENCE}, // not chunchan
        },
        2
    };
    assert(is_elements_chunchan(&elems1));
    assert(!is_elements_chunchan(&elems2));
}

void test_is_elements_routou() {
    Elements elems1 = {
        {
            {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p9,p9,p9}, 3, false, ELEM_TYPE_TRIPLETS},
        },
        2
    };
    Elements elems2 = {
        {
            {{m9,m9,m9}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p1,p2,p3}, 3, false, ELEM_TYPE_SEQUENCE}, // not only routou
        },
        2
    };
    assert(is_elements_routou(&elems1));
    assert(!is_elements_routou(&elems2));
}

void test_has_elements_routou() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p9,p9,p9}, 3, false, ELEM_TYPE_TRIPLETS},
        },
        2
    };
    Elements elems2 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{wt,wt,wt}, 3, false, ELEM_TYPE_TRIPLETS}, // not routou
        },
        2
    };
    assert(has_elements_routou(&elems1));
    assert(!has_elements_routou(&elems2));
}

void test_is_elements_yaochu() {
    Elements elems1 = {
        {
            {{m1,m1,m1}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p9,p9,p9}, 3, false, ELEM_TYPE_TRIPLETS},
            {{dw,dw,dw}, 3, false, ELEM_TYPE_TRIPLETS},
        },
        3
    };
    Elements elems2 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE}, // not only yaochu
            {{s9,s9,s9}, 3, false, ELEM_TYPE_TRIPLETS},
            {{wt,wt,wt}, 3, false, ELEM_TYPE_TRIPLETS},
        },
        3
    };
    assert(is_elements_yaochu(&elems1));
    assert(!is_elements_yaochu(&elems2));
}

void test_has_elements_yaochu() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p7,p8,p9}, 3, false, ELEM_TYPE_SEQUENCE},
            {{dw,dw,dw}, 3, false, ELEM_TYPE_TRIPLETS},
        },
        3
    };
    Elements elems2 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{s9,s9,s9}, 3, false, ELEM_TYPE_TRIPLETS},
            {{wt,wt,wt}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p2,p3,p4}, 3, false, ELEM_TYPE_SEQUENCE}, // not yaochu
        },
        4
    };
    assert(has_elements_yaochu(&elems1));
    assert(!has_elements_yaochu(&elems2));
}

void test_count_elements_sequence() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p7,p7,p7}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p7,p8,p9}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        3
    };
    assert(count_elements_sequence(&elems1) == 2);
}

void test_count_elements_triplets() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p7,p7,p7}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p7,p8,p9}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        3
    };
    assert(count_elements_triplets(&elems1) == 1);
}

void test_count_elements_fours() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p7,p7,p7}, 3, false, ELEM_TYPE_TRIPLETS},
            {{s7,s7,s7,s7}, 3, false, ELEM_TYPE_FOURS},
            {{wp,wp,wp,wp}, 3, false, ELEM_TYPE_FOURS},
        },
        4
    };
    assert(count_elements_fours(&elems1) == 2);
}

void test_count_elements_concealed_fours() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{dg,dg,dg,dg}, 3, true, ELEM_TYPE_FOURS},
            {{s7,s7,s7,s7}, 3, true, ELEM_TYPE_FOURS},
            {{wp,wp,wp,wp}, 3, false, ELEM_TYPE_FOURS},
        },
        4
    };
    assert(count_elements_concealed_fours(&elems1) == 2);
}

void test_has_elements_melded() { /* 暗槓はconcealed扱い */
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{dg,dg,dg,dg}, 3, true, ELEM_TYPE_FOURS},
            {{s7,s7,s7,s7}, 3, true, ELEM_TYPE_FOURS},
            {{wp,wp,wp,wp}, 3, false, ELEM_TYPE_FOURS},
        },
        4
    };
    Elements elems2 = {
        {
            {{dg,dg,dg,dg}, 3, true, ELEM_TYPE_FOURS},
            {{s7,s7,s7,s7}, 3, true, ELEM_TYPE_FOURS},
        },
        2
    };
    assert(has_elements_melded(&elems1));
    assert(!has_elements_melded(&elems2));
}

void test_is_ryanmen_machi() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        1
    };
    Elements elems2 = {
        {
            {{m4,m5,m6}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        1
    };
    Elements elems3 = {
        {
            {{s6,s7,s8}, 3, false, ELEM_TYPE_SEQUENCE},
            {{m7,m8,m9}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        2
    };
    assert(is_ryanmen_machi(&elems1, m1));
    assert(!is_ryanmen_machi(&elems1, m2));
    assert(!is_ryanmen_machi(&elems1, m3));
    assert(is_ryanmen_machi(&elems2, m4));
    assert(!is_ryanmen_machi(&elems2, m5));
    assert(is_ryanmen_machi(&elems2, m6));
    assert(!is_ryanmen_machi(&elems3, m7));
    assert(!is_ryanmen_machi(&elems3, m8));
    assert(is_ryanmen_machi(&elems3, m9));
}

void test_is_shanpon_machi() {
    Elements elems1 = {
        {
            {{m1,m1,m1}, 3, true, ELEM_TYPE_TRIPLETS},
        },
        1
    };
    Elements elems2 = {
        {
            {{s4,s5,s6}, 3, true, ELEM_TYPE_TRIPLETS},
        },
        1
    };
    assert(is_shanpon_machi(&elems1, m1));
    assert(!is_shanpon_machi(&elems2, s6));
}

void test_is_tanki_machi() {
    assert(is_tanki_machi(m1, m1));
    assert(!is_tanki_machi(m2, m1));
}

/*
 * counts same sequence element in elements
 * 123, 123, 123, 123 => 6
 * 123, 123, 123, xxx => 3
 * 123, 123, yyy, xxx => 1
 * 123, zzz, yyy, xxx => 0
 * 123, 456, 123, 456 => 2
 */
void test_count_elements_same_sequence() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p7,p8,p9}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p7,p8,p9}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        3
    };
    Elements elems2 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{s9,s9,s9}, 3, false, ELEM_TYPE_TRIPLETS},
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        4
    };
    Elements elems3 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        4
    };
    Elements elems4 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        4
    };
    Elements elems5 = {
        {
            {{wt,wt,wt}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
            {{wt,wt,wt}, 3, false, ELEM_TYPE_TRIPLETS},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        4
    };
    assert(count_elements_same_sequence(&elems1) == 1);
    assert(count_elements_same_sequence(&elems2) == 3);
    assert(count_elements_same_sequence(&elems3) == 6);
    assert(count_elements_same_sequence(&elems4) == 2);
    assert(count_elements_same_sequence(&elems5) == 1);
}

void test_is_same_elements() {
    {
        MJMelds me1 = {
            {
                {{wt,wt,wt},3,true,-1},
                {{s5,s6,s7},3,true,-1},
                {{p7,p8,p9},3,true,-1},
                {{m3,m4,m5},3,true,-1},
            },
            4,
        };
        MJMelds me2 = {
            {
                {{m3,m4,m5},3,true,-1},
                {{wt,wt,wt},3,true,-1},
                {{p7,p8,p9},3,true,-1},
                {{s5,s6,s7},3,true,-1},
            },
            4,
        };
        Elements e1;
        Elements e2;
        gen_elements_from_melds(&e1, &me1);
        gen_elements_from_melds(&e2, &me2);
        assert(is_same_elements(&e1, &e2));
    }
    {
        MJMelds me1 = {
            {
                {{s5,s5,s5},3,true,-1},
                {{s5,s6,s7},3,true,-1},
                {{p7,p8,p9},3,true,-1},
                {{m3,m4,m5},3,true,-1},
            },
            4,
        };
        MJMelds me2 = {
            {
                {{m3,m4,m5},3,true,-1},
                {{s5,s5,s5},3,true,-1},
                {{p7,p8,p9},3,true,-1},
                {{s5,s6,s7},3,true,-1},
            },
            4,
        };
        Elements e1;
        Elements e2;
        gen_elements_from_melds(&e1, &me1);
        gen_elements_from_melds(&e2, &me2);
        assert(is_same_elements(&e1, &e2));
    }
    {
        MJMelds me1 = {
            {
                {{s5,s5,s5},3,true,-1},
                {{s5,s6,s7},3,true,-1},
                {{p7,p8,p9},3,true,-1},
                {{m3,m4,m5},3,true,-1},
            },
            4,
        };
        MJMelds me2 = {
            {
                {{m3,m4,m5},3,true,-1},
                {{s5,s5,s5},3,true,-1},
                {{p7,p7,p7},3,true,-1},
                {{s5,s6,s7},3,true,-1},
            },
            4,
        };
        Elements e1;
        Elements e2;
        gen_elements_from_melds(&e1, &me1);
        gen_elements_from_melds(&e2, &me2);
        assert(!is_same_elements(&e1, &e2));
    }
}

void test_has_elements_tile_id() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        2
    };
    assert(has_elements_tile_id(&elems1, p7));
    assert(!has_elements_tile_id(&elems1, p9));
}

void test_merge_elements() {
    Elements elems1 = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        2
    };
    Elements elems2 = {
        {
            {{ws,ws,ws}, 3, false, ELEM_TYPE_TRIPLETS},
            {{s6,s7,s8}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        2
    };
    Elements act;
    memset(&act, -1, sizeof(Elements));
    Elements exp = {
        {
            {{m1,m2,m3}, 3, false, ELEM_TYPE_SEQUENCE},
            {{p6,p7,p8}, 3, false, ELEM_TYPE_SEQUENCE},
            {{ws,ws,ws}, 3, false, ELEM_TYPE_TRIPLETS},
            {{s6,s7,s8}, 3, false, ELEM_TYPE_SEQUENCE},
        },
        4
    };
    merge_elements(&act, &elems1, &elems2);
    assert(memcmp(&act, &exp, sizeof(Elements)) == 0);
}

bool test_element() {
    test_gen_elements_from_melds();
    test_is_element_chunchan();
    test_is_element_routou();
    test_has_element_routou();
    test_is_element_yaochu();
    test_has_element_yaochu();
    test_is_element_sequence();
    test_is_element_triplets();
    test_is_element_fours();
    test_is_element_concealed();
    test_is_elements_chunchan();
    test_is_elements_routou();
    test_has_elements_routou();
    test_is_elements_yaochu();
    test_has_elements_yaochu();
    test_count_elements_sequence();
    test_count_elements_triplets();
    test_count_elements_fours();
    test_count_elements_concealed_fours();
    test_has_elements_melded();
    test_is_ryanmen_machi();
    test_is_shanpon_machi();
    test_is_tanki_machi();
    test_count_elements_same_sequence();
    test_is_same_elements();
    test_has_elements_tile_id();
    test_merge_elements();
    return true;
}
