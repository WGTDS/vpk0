                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             void __stdcall FUN_80103530(int param_1, undefined * param_2)
                               assume gp = 0x1
             void              <VOID>         <RETURN>
             int               a0:4           param_1
             undefined *       a1:4           param_2
             undefined4        Stack[-0x304   local_304                               XREF[4]:     80103600(W), 
                                                                                                   80103640(W), 
                                                                                                   801036d0(W), 
                                                                                                   80103710(W)  
             undefined4        Stack[-0x308   local_308                               XREF[4]:     801035fc(W), 
                                                                                                   80103638(W), 
                                                                                                   801036cc(W), 
                                                                                                   80103708(W)  
             undefined4        Stack[-0x30c   local_30c                               XREF[4]:     801035f8(W), 
                                                                                                   80103634(W), 
                                                                                                   801036c8(W), 
                                                                                                   80103704(W)  
             undefined4        Stack[-0x324   local_324                               XREF[2]:     801035a4(W), 
                                                                                                   80103788(R)  
             undefined4        Stack[-0x374   local_374                               XREF[3]:     801035ac(W), 
                                                                                                   8010366c(W), 
                                                                                                   80103678(R)  
             undefined4        Stack[-0x378   local_378                               XREF[1]:     80103614(R)  
             undefined4        Stack[-0x37c   local_37c                               XREF[2]:     80103604(R), 
                                                                                                   80103624(W)  
             undefined4        Stack[-0x3d4   local_3d4                               XREF[3]:     80103680(W), 
                                                                                                   8010373c(W), 
                                                                                                   80103750(R)  
             undefined4        Stack[-0x3d8   local_3d8                               XREF[1]:     801036e4(R)  
             undefined4        Stack[-0x3dc   local_3dc                               XREF[2]:     801036d4(R), 
                                                                                                   801036f4(W)  
             undefined4        Stack[-0x41c   local_41c                               XREF[2]:     80103534(W), 
                                                                                                   80103a70(R)  
             undefined4        Stack[-0x420   local_420                               XREF[2]:     80103538(W), 
                                                                                                   80103a6c(R)  
             undefined4        Stack[-0x424   local_424                               XREF[2]:     8010353c(W), 
                                                                                                   80103a68(R)  
             undefined4        Stack[-0x428   local_428                               XREF[2]:     80103540(W), 
                                                                                                   80103a64(R)  
                             FUN_80103530
        80103530 27 bd fb d0     addiu      sp,sp,-0x430
             assume gp = <UNKNOWN>
        80103534 af b3 00 14     sw         s3,local_41c(sp)
        80103538 af b2 00 10     sw         s2,local_420(sp)
        8010353c af b1 00 0c     sw         s1,local_424(sp)
        80103540 af b0 00 08     sw         s0,local_428(sp)
        80103544 94 83 00 00     lhu        v1,0x0(param_1)
        80103548 94 8e 00 02     lhu        t6,0x2(param_1)
        8010354c 94 8f 00 04     lhu        t7,0x4(param_1)
        80103550 00 03 1c 00     sll        v1,v1,0x10
        80103554 00 6e 18 25     or         v1,v1,t6
        80103558 94 98 00 06     lhu        t8,0x6(param_1)
        8010355c 00 03 1c 00     sll        v1,v1,0x10
        80103560 00 6f 18 25     or         v1,v1,t7
        80103564 00 03 1c 00     sll        v1,v1,0x10
        80103568 00 80 48 25     or         t1,param_1,zero
        8010356c 95 39 00 08     lhu        t9,0x8(t1)
        80103570 00 78 18 25     or         v1,v1,t8
        80103574 00 65 68 21     addu       t5,v1,param_2
        80103578 00 00 10 25     or         v0,zero,zero
        8010357c 24 42 00 08     addiu      v0,v0,0x8
        80103580 00 03 1c 00     sll        v1,v1,0x10
        80103584 24 0b 00 18     li         t3,0x18
        80103588 01 62 70 23     subu       t6,t3,v0
        8010358c 00 79 18 25     or         v1,v1,t9
        80103590 01 c3 78 04     sllv       t7,v1,t6
        80103594 00 0f c6 02     srl        t8,t7,0x18
        80103598 25 24 00 0a     addiu      param_1,t1,0xa
        8010359c 27 a6 01 24     addiu      a2,sp,0x124
        801035a0 00 a0 40 25     or         t0,param_2,zero
        801035a4 af b8 01 0c     sw         t8,local_324(sp)
        801035a8 00 00 48 25     or         t1,zero,zero
        801035ac af a0 00 bc     sw         zero,local_374(sp)
        801035b0 24 07 00 1f     li         a3,0x1f
                             LAB_801035b4                                    XREF[2]:     80103620(j), 80103670(j)  
        801035b4 1c 40 00 06     bgtz       v0,LAB_801035d0
        801035b8 29 21 00 02     _slti      at,t1,0x2
        801035bc 94 99 00 00     lhu        t9,0x0(param_1)
        801035c0 00 03 1c 00     sll        v1,v1,0x10
        801035c4 24 84 00 02     addiu      param_1,param_1,0x2
        801035c8 24 42 00 10     addiu      v0,v0,0x10
        801035cc 00 79 18 25     or         v1,v1,t9
                             LAB_801035d0                                    XREF[1]:     801035b4(j)  
        801035d0 24 42 ff ff     addiu      v0,v0,-0x1
        801035d4 00 e2 70 23     subu       t6,a3,v0
        801035d8 01 c3 50 04     sllv       t2,v1,t6
        801035dc 00 0a 57 c2     srl        t2,t2,0x1f
        801035e0 11 40 00 02     beq        t2,zero,LAB_801035ec
        801035e4 00 09 78 80     _sll       t7,t1,0x2
        801035e8 14 20 00 23     bne        at,zero,LAB_80103678
                             LAB_801035ec                                    XREF[1]:     801035e0(j)  
        801035ec 27 b8 00 bc     _addiu     t8,sp,0xbc
        801035f0 11 40 00 0d     beq        t2,zero,LAB_80103628
        801035f4 01 f8 60 21     _addu      t4,t7,t8
        801035f8 ac c0 00 00     sw         zero,0x0(a2)=>local_30c
        801035fc ac c0 00 04     sw         zero,local_308(a2)
        80103600 ac c0 00 08     sw         zero,local_304(a2)
        80103604 8d 99 ff f8     lw         t9,local_37c(t4)
        80103608 00 c0 50 25     or         t2,a2,zero
        8010360c 24 c6 00 0c     addiu      a2,a2,0xc
        80103610 ad 59 00 00     sw         t9,0x0(t2)
        80103614 8d 8e ff fc     lw         t6,local_378(t4)
        80103618 25 29 ff ff     addiu      t1,t1,-0x1
        8010361c ad 4e 00 04     sw         t6,0x4(t2)
        80103620 10 00 ff e4     b          LAB_801035b4
        80103624 ad 8a ff f8     _sw        t2,local_37c(t4)
                             LAB_80103628                                    XREF[1]:     801035f0(j)  
        80103628 00 c0 50 25     or         t2,a2,zero
        8010362c 28 41 00 08     slti       at,v0,0x8
        80103630 24 c6 00 0c     addiu      a2,a2,0xc
        80103634 ac c0 ff f4     sw         zero,local_30c(a2)
        80103638 ac c0 ff f8     sw         zero,local_308(a2)
        8010363c 10 20 00 06     beq        at,zero,LAB_80103658
        80103640 ac c0 ff fc     _sw        zero,local_304(a2)
        80103644 94 8f 00 00     lhu        t7,0x0(param_1)
        80103648 00 03 1c 00     sll        v1,v1,0x10
        8010364c 24 84 00 02     addiu      param_1,param_1,0x2
        80103650 24 42 00 10     addiu      v0,v0,0x10
        80103654 00 6f 18 25     or         v1,v1,t7
                             LAB_80103658                                    XREF[1]:     8010363c(j)  
        80103658 24 42 ff f8     addiu      v0,v0,-0x8
        8010365c 01 62 c0 23     subu       t8,t3,v0
        80103660 03 03 c8 04     sllv       t9,v1,t8
        80103664 00 19 76 02     srl        t6,t9,0x18
        80103668 ad 4e 00 08     sw         t6,0x8(t2)
        8010366c ad 8a 00 00     sw         t2,0x0(t4)=>local_374
        80103670 10 00 ff d0     b          LAB_801035b4
        80103674 25 29 00 01     _addiu     t1,t1,0x1
                             LAB_80103678                                    XREF[1]:     801035e8(j)  
        80103678 8f a9 00 bc     lw         t1,local_374(sp)
        8010367c 00 00 60 25     or         t4,zero,zero
        80103680 af a0 00 5c     sw         zero,local_3d4(sp)
                             LAB_80103684                                    XREF[2]:     801036f0(j), 80103740(j)  
        80103684 1c 40 00 06     bgtz       v0,LAB_801036a0
        80103688 29 81 00 02     _slti      at,t4,0x2
        8010368c 94 8f 00 00     lhu        t7,0x0(param_1)
        80103690 00 03 1c 00     sll        v1,v1,0x10
        80103694 24 84 00 02     addiu      param_1,param_1,0x2
        80103698 24 42 00 10     addiu      v0,v0,0x10
        8010369c 00 6f 18 25     or         v1,v1,t7
                             LAB_801036a0                                    XREF[1]:     80103684(j)  
        801036a0 24 42 ff ff     addiu      v0,v0,-0x1
        801036a4 00 e2 c0 23     subu       t8,a3,v0
        801036a8 03 03 50 04     sllv       t2,v1,t8
        801036ac 00 0a 57 c2     srl        t2,t2,0x1f
        801036b0 11 40 00 02     beq        t2,zero,LAB_801036bc
        801036b4 00 0c c8 80     _sll       t9,t4,0x2
        801036b8 14 20 00 23     bne        at,zero,LAB_80103748
                             LAB_801036bc                                    XREF[1]:     801036b0(j)  
        801036bc 27 ae 00 5c     _addiu     t6,sp,0x5c
        801036c0 11 40 00 0d     beq        t2,zero,LAB_801036f8
        801036c4 03 2e 80 21     _addu      s0,t9,t6
        801036c8 ac c0 00 00     sw         zero,0x0(a2)=>local_30c
        801036cc ac c0 00 04     sw         zero,local_308(a2)
        801036d0 ac c0 00 08     sw         zero,local_304(a2)
        801036d4 8e 0f ff f8     lw         t7,local_3dc(s0)
        801036d8 00 c0 50 25     or         t2,a2,zero
        801036dc 24 c6 00 0c     addiu      a2,a2,0xc
        801036e0 ad 4f 00 00     sw         t7,0x0(t2)
        801036e4 8e 18 ff fc     lw         t8,local_3d8(s0)
        801036e8 25 8c ff ff     addiu      t4,t4,-0x1
        801036ec ad 58 00 04     sw         t8,0x4(t2)
        801036f0 10 00 ff e4     b          LAB_80103684
        801036f4 ae 0a ff f8     _sw        t2,local_3dc(s0)
                             LAB_801036f8                                    XREF[1]:     801036c0(j)  
        801036f8 00 c0 50 25     or         t2,a2,zero
        801036fc 28 41 00 08     slti       at,v0,0x8
        80103700 24 c6 00 0c     addiu      a2,a2,0xc
        80103704 ac c0 ff f4     sw         zero,local_30c(a2)
        80103708 ac c0 ff f8     sw         zero,local_308(a2)
        8010370c 10 20 00 06     beq        at,zero,LAB_80103728
        80103710 ac c0 ff fc     _sw        zero,local_304(a2)
        80103714 94 99 00 00     lhu        t9,0x0(param_1)
        80103718 00 03 1c 00     sll        v1,v1,0x10
        8010371c 24 84 00 02     addiu      param_1,param_1,0x2
        80103720 24 42 00 10     addiu      v0,v0,0x10
        80103724 00 79 18 25     or         v1,v1,t9
                             LAB_80103728                                    XREF[1]:     8010370c(j)  
        80103728 24 42 ff f8     addiu      v0,v0,-0x8
        8010372c 01 62 70 23     subu       t6,t3,v0
        80103730 01 c3 78 04     sllv       t7,v1,t6
        80103734 00 0f c6 02     srl        t8,t7,0x18
        80103738 ad 58 00 08     sw         t8,0x8(t2)
        8010373c ae 0a 00 00     sw         t2,0x0(s0)=>local_3d4
        80103740 10 00 ff d0     b          LAB_80103684
        80103744 25 8c 00 01     _addiu     t4,t4,0x1
                             LAB_80103748                                    XREF[1]:     801036b8(j)  
        80103748 00 ad 08 2b     sltu       at,param_2,t5
        8010374c 10 20 00 c5     beq        at,zero,LAB_80103a64
        80103750 8f ac 00 5c     _lw        t4,local_3d4(sp)
        80103754 24 06 00 20     li         a2,0x20
                             LAB_80103758                                    XREF[1]:     80103a5c(j)  
        80103758 5c 40 00 07     bgtzl      v0,LAB_80103778
        8010375c 24 42 ff ff     _addiu     v0,v0,-0x1
        80103760 94 99 00 00     lhu        t9,0x0(param_1)
        80103764 00 03 1c 00     sll        v1,v1,0x10
        80103768 24 84 00 02     addiu      param_1,param_1,0x2
        8010376c 24 42 00 10     addiu      v0,v0,0x10
        80103770 00 79 18 25     or         v1,v1,t9
        80103774 24 42 ff ff     addiu      v0,v0,-0x1
                             LAB_80103778                                    XREF[1]:     80103758(j)  
        80103778 00 e2 70 23     subu       t6,a3,v0
        8010377c 01 c3 78 04     sllv       t7,v1,t6
        80103780 00 0f c7 c2     srl        t8,t7,0x1f
        80103784 17 00 00 10     bne        t8,zero,LAB_801037c8
        80103788 8f b9 01 0c     _lw        t9,local_324(sp)
        8010378c 28 41 00 08     slti       at,v0,0x8
        80103790 50 20 00 07     beql       at,zero,LAB_801037b0
        80103794 24 42 ff f8     _addiu     v0,v0,-0x8
        80103798 94 99 00 00     lhu        t9,0x0(param_1)
        8010379c 00 03 1c 00     sll        v1,v1,0x10
        801037a0 24 84 00 02     addiu      param_1,param_1,0x2
        801037a4 24 42 00 10     addiu      v0,v0,0x10
        801037a8 00 79 18 25     or         v1,v1,t9
        801037ac 24 42 ff f8     addiu      v0,v0,-0x8
                             LAB_801037b0                                    XREF[1]:     80103790(j)  
        801037b0 01 62 70 23     subu       t6,t3,v0
        801037b4 01 c3 78 04     sllv       t7,v1,t6
        801037b8 00 0f c6 02     srl        t8,t7,0x18
        801037bc a1 18 00 00     sb         t8,0x0(t0)
        801037c0 10 00 00 a5     b          LAB_80103a58
        801037c4 25 08 00 01     _addiu     t0,t0,0x1
                             LAB_801037c8                                    XREF[1]:     80103784(j)  
        801037c8 8d 32 00 00     lw         s2,0x0(t1)
        801037cc 01 80 28 25     or         param_2,t4,zero
        801037d0 13 20 00 50     beq        t9,zero,LAB_80103914
        801037d4 8d 8a 00 00     _lw        t2,0x0(t4)
        801037d8 00 00 80 25     or         s0,zero,zero
        801037dc 12 40 00 14     beq        s2,zero,LAB_80103830
        801037e0 01 20 88 25     _or        s1,t1,zero
                             LAB_801037e4                                    XREF[1]:     80103828(j)  
        801037e4 5c 40 00 07     bgtzl      v0,LAB_80103804
        801037e8 24 42 ff ff     _addiu     v0,v0,-0x1
        801037ec 94 8e 00 00     lhu        t6,0x0(param_1)
        801037f0 00 03 1c 00     sll        v1,v1,0x10
        801037f4 24 84 00 02     addiu      param_1,param_1,0x2
        801037f8 24 42 00 10     addiu      v0,v0,0x10
        801037fc 00 6e 18 25     or         v1,v1,t6
        80103800 24 42 ff ff     addiu      v0,v0,-0x1
                             LAB_80103804                                    XREF[1]:     801037e4(j)  
        80103804 00 e2 78 23     subu       t7,a3,v0
        80103808 01 e3 c0 04     sllv       t8,v1,t7
        8010380c 00 18 cf c2     srl        t9,t8,0x1f
        80103810 57 20 00 04     bnel       t9,zero,LAB_80103824
        80103814 8e 31 00 04     _lw        s1,0x4(s1)
        80103818 10 00 00 02     b          LAB_80103824
        8010381c 8e 31 00 00     _lw        s1,0x0(s1)
                             LAB_80103824                                    XREF[2]:     80103810(j), 80103818(j)  
        80103824 8e 2e 00 00     lw         t6,0x0(s1)
        80103828 15 c0 ff ee     bne        t6,zero,LAB_801037e4
        8010382c 00 00 00 00     _nop
                             LAB_80103830                                    XREF[1]:     801037dc(j)  
        80103830 8e 33 00 08     lw         s3,0x8(s1)
        80103834 00 53 08 2a     slt        at,v0,s3
        80103838 10 20 00 06     beq        at,zero,LAB_80103854
        8010383c 00 d3 c0 23     _subu      t8,a2,s3
        80103840 94 8f 00 00     lhu        t7,0x0(param_1)
        80103844 00 03 1c 00     sll        v1,v1,0x10
        80103848 24 84 00 02     addiu      param_1,param_1,0x2
        8010384c 24 42 00 10     addiu      v0,v0,0x10
        80103850 00 6f 18 25     or         v1,v1,t7
                             LAB_80103854                                    XREF[1]:     80103838(j)  
        80103854 00 53 10 23     subu       v0,v0,s3
        80103858 03 02 c8 23     subu       t9,t8,v0
        8010385c 03 23 70 04     sllv       t6,v1,t9
        80103860 00 d3 78 23     subu       t7,a2,s3
        80103864 01 ee 88 06     srlv       s1,t6,t7
        80103868 2a 21 00 03     slti       at,s1,0x3
        8010386c 10 20 00 24     beq        at,zero,LAB_80103900
        80103870 01 20 98 25     _or        s3,t1,zero
        80103874 12 40 00 14     beq        s2,zero,LAB_801038c8
        80103878 26 30 00 01     _addiu     s0,s1,0x1
                             LAB_8010387c                                    XREF[1]:     801038c0(j)  
        8010387c 5c 40 00 07     bgtzl      v0,LAB_8010389c
        80103880 24 42 ff ff     _addiu     v0,v0,-0x1
        80103884 94 98 00 00     lhu        t8,0x0(param_1)
        80103888 00 03 1c 00     sll        v1,v1,0x10
        8010388c 24 84 00 02     addiu      param_1,param_1,0x2
        80103890 24 42 00 10     addiu      v0,v0,0x10
        80103894 00 78 18 25     or         v1,v1,t8
        80103898 24 42 ff ff     addiu      v0,v0,-0x1
                             LAB_8010389c                                    XREF[1]:     8010387c(j)  
        8010389c 00 e2 c8 23     subu       t9,a3,v0
        801038a0 03 23 70 04     sllv       t6,v1,t9
        801038a4 00 0e 7f c2     srl        t7,t6,0x1f
        801038a8 55 e0 00 04     bnel       t7,zero,LAB_801038bc
        801038ac 8e 73 00 04     _lw        s3,0x4(s3)
        801038b0 10 00 00 02     b          LAB_801038bc
        801038b4 8e 73 00 00     _lw        s3,0x0(s3)
                             LAB_801038bc                                    XREF[2]:     801038a8(j), 801038b0(j)  
        801038bc 8e 78 00 00     lw         t8,0x0(s3)
        801038c0 17 00 ff ee     bne        t8,zero,LAB_8010387c
        801038c4 00 00 00 00     _nop
                             LAB_801038c8                                    XREF[1]:     80103874(j)  
        801038c8 8e 72 00 08     lw         s2,0x8(s3)
        801038cc 00 52 08 2a     slt        at,v0,s2
        801038d0 10 20 00 06     beq        at,zero,LAB_801038ec
        801038d4 00 d2 70 23     _subu      t6,a2,s2
        801038d8 94 99 00 00     lhu        t9,0x0(param_1)
        801038dc 00 03 1c 00     sll        v1,v1,0x10
        801038e0 24 84 00 02     addiu      param_1,param_1,0x2
        801038e4 24 42 00 10     addiu      v0,v0,0x10
        801038e8 00 79 18 25     or         v1,v1,t9
                             LAB_801038ec                                    XREF[1]:     801038d0(j)  
        801038ec 00 52 10 23     subu       v0,v0,s2
        801038f0 01 c2 78 23     subu       t7,t6,v0
        801038f4 01 e3 c0 04     sllv       t8,v1,t7
        801038f8 00 d2 c8 23     subu       t9,a2,s2
        801038fc 03 38 88 06     srlv       s1,t8,t9
                             LAB_80103900                                    XREF[1]:     8010386c(j)  
        80103900 00 11 70 80     sll        t6,s1,0x2
        80103904 01 0e 78 23     subu       t7,t0,t6
        80103908 01 f0 90 23     subu       s2,t7,s0
        8010390c 10 00 00 25     b          LAB_801039a4
        80103910 26 52 00 08     _addiu     s2,s2,0x8
                             LAB_80103914                                    XREF[1]:     801037d0(j)  
        80103914 12 40 00 14     beq        s2,zero,LAB_80103968
        80103918 01 20 80 25     _or        s0,t1,zero
                             LAB_8010391c                                    XREF[1]:     80103960(j)  
        8010391c 5c 40 00 07     bgtzl      v0,LAB_8010393c
        80103920 24 42 ff ff     _addiu     v0,v0,-0x1
        80103924 94 98 00 00     lhu        t8,0x0(param_1)
        80103928 00 03 1c 00     sll        v1,v1,0x10
        8010392c 24 84 00 02     addiu      param_1,param_1,0x2
        80103930 24 42 00 10     addiu      v0,v0,0x10
        80103934 00 78 18 25     or         v1,v1,t8
        80103938 24 42 ff ff     addiu      v0,v0,-0x1
                             LAB_8010393c                                    XREF[1]:     8010391c(j)  
        8010393c 00 e2 c8 23     subu       t9,a3,v0
        80103940 03 23 70 04     sllv       t6,v1,t9
        80103944 00 0e 7f c2     srl        t7,t6,0x1f
        80103948 55 e0 00 04     bnel       t7,zero,LAB_8010395c
        8010394c 8e 10 00 04     _lw        s0,0x4(s0)
        80103950 10 00 00 02     b          LAB_8010395c
        80103954 8e 10 00 00     _lw        s0,0x0(s0)
                             LAB_8010395c                                    XREF[2]:     80103948(j), 80103950(j)  
        8010395c 8e 18 00 00     lw         t8,0x0(s0)
        80103960 17 00 ff ee     bne        t8,zero,LAB_8010391c
        80103964 00 00 00 00     _nop
                             LAB_80103968                                    XREF[1]:     80103914(j)  
        80103968 8e 13 00 08     lw         s3,0x8(s0)
        8010396c 00 53 08 2a     slt        at,v0,s3
        80103970 10 20 00 06     beq        at,zero,LAB_8010398c
        80103974 00 d3 70 23     _subu      t6,a2,s3
        80103978 94 99 00 00     lhu        t9,0x0(param_1)
        8010397c 00 03 1c 00     sll        v1,v1,0x10
        80103980 24 84 00 02     addiu      param_1,param_1,0x2
        80103984 24 42 00 10     addiu      v0,v0,0x10
        80103988 00 79 18 25     or         v1,v1,t9
                             LAB_8010398c                                    XREF[1]:     80103970(j)  
        8010398c 00 53 10 23     subu       v0,v0,s3
        80103990 01 c2 78 23     subu       t7,t6,v0
        80103994 01 e3 c0 04     sllv       t8,v1,t7
        80103998 00 d3 c8 23     subu       t9,a2,s3
        8010399c 03 38 88 06     srlv       s1,t8,t9
        801039a0 01 11 90 23     subu       s2,t0,s1
                             LAB_801039a4                                    XREF[1]:     8010390c(j)  
        801039a4 51 40 00 15     beql       t2,zero,LAB_801039fc
        801039a8 8c aa 00 08     _lw        t2,0x8(param_2)
                             LAB_801039ac                                    XREF[1]:     801039f0(j)  
        801039ac 5c 40 00 07     bgtzl      v0,LAB_801039cc
        801039b0 24 42 ff ff     _addiu     v0,v0,-0x1
        801039b4 94 8e 00 00     lhu        t6,0x0(param_1)
        801039b8 00 03 1c 00     sll        v1,v1,0x10
        801039bc 24 84 00 02     addiu      param_1,param_1,0x2
        801039c0 24 42 00 10     addiu      v0,v0,0x10
        801039c4 00 6e 18 25     or         v1,v1,t6
        801039c8 24 42 ff ff     addiu      v0,v0,-0x1
                             LAB_801039cc                                    XREF[1]:     801039ac(j)  
        801039cc 00 e2 78 23     subu       t7,a3,v0
        801039d0 01 e3 c0 04     sllv       t8,v1,t7
        801039d4 00 18 cf c2     srl        t9,t8,0x1f
        801039d8 57 20 00 04     bnel       t9,zero,LAB_801039ec
        801039dc 8c a5 00 04     _lw        param_2,0x4(param_2)
        801039e0 10 00 00 02     b          LAB_801039ec
        801039e4 8c a5 00 00     _lw        param_2,0x0(param_2)
                             LAB_801039ec                                    XREF[2]:     801039d8(j), 801039e0(j)  
        801039ec 8c ae 00 00     lw         t6,0x0(param_2)
        801039f0 15 c0 ff ee     bne        t6,zero,LAB_801039ac
        801039f4 00 00 00 00     _nop
        801039f8 8c aa 00 08     lw         t2,0x8(param_2)
                             LAB_801039fc                                    XREF[1]:     801039a4(j)  
        801039fc 00 4a 08 2a     slt        at,v0,t2
        80103a00 10 20 00 06     beq        at,zero,LAB_80103a1c
        80103a04 00 ca c0 23     _subu      t8,a2,t2
        80103a08 94 8f 00 00     lhu        t7,0x0(param_1)
        80103a0c 00 03 1c 00     sll        v1,v1,0x10
        80103a10 24 84 00 02     addiu      param_1,param_1,0x2
        80103a14 24 42 00 10     addiu      v0,v0,0x10
        80103a18 00 6f 18 25     or         v1,v1,t7
                             LAB_80103a1c                                    XREF[1]:     80103a00(j)  
        80103a1c 00 4a 10 23     subu       v0,v0,t2
        80103a20 03 02 c8 23     subu       t9,t8,v0
        80103a24 03 23 70 04     sllv       t6,v1,t9
        80103a28 00 ca 78 23     subu       t7,a2,t2
        80103a2c 01 ee 28 06     srlv       param_2,t6,t7
        80103a30 00 05 80 2a     slt        s0,zero,param_2
        80103a34 12 00 00 08     beq        s0,zero,LAB_80103a58
        80103a38 24 a5 ff ff     _addiu     param_2,param_2,-0x1
                             LAB_80103a3c                                    XREF[1]:     80103a50(j)  
        80103a3c 92 58 00 00     lbu        t8,0x0(s2)
        80103a40 00 05 80 2a     slt        s0,zero,param_2
        80103a44 24 a5 ff ff     addiu      param_2,param_2,-0x1
        80103a48 25 08 00 01     addiu      t0,t0,0x1
        80103a4c 26 52 00 01     addiu      s2,s2,0x1
        80103a50 16 00 ff fa     bne        s0,zero,LAB_80103a3c
        80103a54 a1 18 ff ff     _sb        t8,-0x1(t0)
                             LAB_80103a58                                    XREF[2]:     801037c0(j), 80103a34(j)  
        80103a58 01 0d 08 2b     sltu       at,t0,t5
        80103a5c 14 20 ff 3e     bne        at,zero,LAB_80103758
        80103a60 00 00 00 00     _nop
                             LAB_80103a64                                    XREF[1]:     8010374c(j)  
        80103a64 8f b0 00 08     lw         s0,local_428(sp)
        80103a68 8f b1 00 0c     lw         s1,local_424(sp)
        80103a6c 8f b2 00 10     lw         s2,local_420(sp)
        80103a70 8f b3 00 14     lw         s3,local_41c(sp)
        80103a74 03 e0 00 08     jr         ra
        80103a78 27 bd 04 30     _addiu     sp,sp,0x430
