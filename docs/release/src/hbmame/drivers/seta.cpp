// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/seta.cpp"

// PSmame (c) gaston90 used with permission

 /****************************************************
         Proyecto Shadows Mame Build Plus
*****************************************************/

 /*******************
 The FairyLand Story   
**********************/

ROM_START( blandias01 )
	ROM_REGION( 0x200000, "maincpu", 0 )  
	ROM_LOAD16_BYTE( "ux001001_ps01.u3", 0x000000, 0x040000, CRC(3020be51) SHA1(0e00fa720e23271cacb745830d41a0070476f935) )
	ROM_LOAD16_BYTE( "ux001002_ps01.u4", 0x000001, 0x040000, CRC(ee030ea6) SHA1(dd917cc5bee20368257331ede395e2774fe094ea) )
	ROM_LOAD16_WORD_SWAP( "ux001003.u202",    0x100000, 0x100000, CRC(98052c63) SHA1(b523596de29038b3ec9f1b6e1f7374a6a8709d42) )

	ROM_REGION( 0x400000, "gfx1", 0 ) 
	ROM_LOAD( "ux001005.u200", 0x300000, 0x100000, CRC(bea0c4a5) SHA1(a690c17fb7cbdab533c1dfad13abbad9359b9631) )
	ROM_LOAD( "ux001007.u201", 0x100000, 0x100000, CRC(4440fdd1) SHA1(7bfee90f81a2c867bd487abcf5905393ad400902) )
	ROM_LOAD( "ux001006.u63",  0x200000, 0x100000, CRC(abc01cf7) SHA1(c3f26e75eeb68073d2825be8df82cc6afcfbfb26) )
	ROM_LOAD( "ux001008.u64",  0x000000, 0x100000, CRC(413647b6) SHA1(594e010ca6f49ec82cc6d44fe23ac3427c4c3dbd) )

	ROM_REGION( 0x0c0000, "gfx2", 0 ) 
	ROM_LOAD( "ux001009.u65", 0x000000, 0x080000, CRC(bc6f6aea) SHA1(673efa0c70587b5650ccf0a3c4bc316f53d52ba6) )
	ROM_LOAD( "ux001010.u66", 0x040000, 0x080000, CRC(bd7f7614) SHA1(dc865ff0f327f460956915b2018aaac815e8fce5) )

	ROM_REGION( 0x0c0000, "gfx3", 0 ) 
	ROM_LOAD( "ux001011.u67",  0x000000, 0x080000, CRC(5efe0397) SHA1(a294a2dae9a10e93912543a8614a7f960a011f27) )
	ROM_LOAD( "ux001012.u068", 0x040000, 0x080000, CRC(f29959f6) SHA1(edccea3d0bf972a07edd6339e18792d089033bff) )

	ROM_REGION( 0x240000, "x1snd", 0 ) 
	ROM_LOAD( "ux001013.u69", 0x000000, 0x0c0000, CRC(5cd273cd) SHA1(602e1f10454e2b1c941f2e6983872bb9ca77a542) )

	ROM_CONTINUE(             0x100000, 0x040000  )
	ROM_LOAD( "ux001014.u70", 0x140000, 0x080000, CRC(86b49b4e) SHA1(045b352950d848907af4c22b817d154b2cfff382) )
ROM_END

ROM_START( blandias02 )
	ROM_REGION( 0x200000, "maincpu", 0 )  
	ROM_LOAD16_BYTE( "ux001001_ps02.u3", 0x000000, 0x040000, CRC(245d404d) SHA1(708dede3bb37e1a90fbd196cf7297f1c5c4fc518) )
	ROM_LOAD16_BYTE( "ux001002_ps02.u4", 0x000001, 0x040000, CRC(9aa9fb5c) SHA1(24ead69c38573ea2b0f1cc8de5c46adb712e5b3b) )
	ROM_LOAD16_WORD_SWAP( "ux001003.u202",    0x100000, 0x100000, CRC(98052c63) SHA1(b523596de29038b3ec9f1b6e1f7374a6a8709d42) )

	ROM_REGION( 0x400000, "gfx1", 0 ) 
	ROM_LOAD( "ux001005.u200", 0x300000, 0x100000, CRC(bea0c4a5) SHA1(a690c17fb7cbdab533c1dfad13abbad9359b9631) )
	ROM_LOAD( "ux001007.u201", 0x100000, 0x100000, CRC(4440fdd1) SHA1(7bfee90f81a2c867bd487abcf5905393ad400902) )
	ROM_LOAD( "ux001006.u63",  0x200000, 0x100000, CRC(abc01cf7) SHA1(c3f26e75eeb68073d2825be8df82cc6afcfbfb26) )
	ROM_LOAD( "ux001008.u64",  0x000000, 0x100000, CRC(413647b6) SHA1(594e010ca6f49ec82cc6d44fe23ac3427c4c3dbd) )

	ROM_REGION( 0x0c0000, "gfx2", 0 ) 
	ROM_LOAD( "ux001009.u65", 0x000000, 0x080000, CRC(bc6f6aea) SHA1(673efa0c70587b5650ccf0a3c4bc316f53d52ba6) )
	ROM_LOAD( "ux001010.u66", 0x040000, 0x080000, CRC(bd7f7614) SHA1(dc865ff0f327f460956915b2018aaac815e8fce5) )

	ROM_REGION( 0x0c0000, "gfx3", 0 ) 
	ROM_LOAD( "ux001011.u67",  0x000000, 0x080000, CRC(5efe0397) SHA1(a294a2dae9a10e93912543a8614a7f960a011f27) )
	ROM_LOAD( "ux001012.u068", 0x040000, 0x080000, CRC(f29959f6) SHA1(edccea3d0bf972a07edd6339e18792d089033bff) )

	ROM_REGION( 0x240000, "x1snd", 0 ) 
	ROM_LOAD( "ux001013.u69", 0x000000, 0x0c0000, CRC(5cd273cd) SHA1(602e1f10454e2b1c941f2e6983872bb9ca77a542) )

	ROM_CONTINUE(             0x100000, 0x040000  )
	ROM_LOAD( "ux001014.u70", 0x140000, 0x080000, CRC(86b49b4e) SHA1(045b352950d848907af4c22b817d154b2cfff382) )
ROM_END

ROM_START( blandias03 )
	ROM_REGION( 0x200000, "maincpu", 0 )  
	ROM_LOAD16_BYTE( "ux001001_ps03.u3", 0x000000, 0x040000, CRC(370b5fef) SHA1(39b310e7c1f5e4b17d618b2ad583697cbbdc4936) )
	ROM_LOAD16_BYTE( "ux001002_ps03.u4", 0x000001, 0x040000, CRC(cdbf1488) SHA1(ed3c0ecf0d15ece35d54e8d6be64f6091328505e) )
	ROM_LOAD16_WORD_SWAP( "ux001003.u202",    0x100000, 0x100000, CRC(98052c63) SHA1(b523596de29038b3ec9f1b6e1f7374a6a8709d42) )

	ROM_REGION( 0x400000, "gfx1", 0 ) 
	ROM_LOAD( "ux001005.u200", 0x300000, 0x100000, CRC(bea0c4a5) SHA1(a690c17fb7cbdab533c1dfad13abbad9359b9631) )
	ROM_LOAD( "ux001007.u201", 0x100000, 0x100000, CRC(4440fdd1) SHA1(7bfee90f81a2c867bd487abcf5905393ad400902) )
	ROM_LOAD( "ux001006.u63",  0x200000, 0x100000, CRC(abc01cf7) SHA1(c3f26e75eeb68073d2825be8df82cc6afcfbfb26) )
	ROM_LOAD( "ux001008.u64",  0x000000, 0x100000, CRC(413647b6) SHA1(594e010ca6f49ec82cc6d44fe23ac3427c4c3dbd) )

	ROM_REGION( 0x0c0000, "gfx2", 0 ) 
	ROM_LOAD( "ux001009.u65", 0x000000, 0x080000, CRC(bc6f6aea) SHA1(673efa0c70587b5650ccf0a3c4bc316f53d52ba6) )
	ROM_LOAD( "ux001010.u66", 0x040000, 0x080000, CRC(bd7f7614) SHA1(dc865ff0f327f460956915b2018aaac815e8fce5) )

	ROM_REGION( 0x0c0000, "gfx3", 0 ) 
	ROM_LOAD( "ux001011.u67",  0x000000, 0x080000, CRC(5efe0397) SHA1(a294a2dae9a10e93912543a8614a7f960a011f27) )
	ROM_LOAD( "ux001012.u068", 0x040000, 0x080000, CRC(f29959f6) SHA1(edccea3d0bf972a07edd6339e18792d089033bff) )

	ROM_REGION( 0x240000, "x1snd", 0 ) 
	ROM_LOAD( "ux001013.u69", 0x000000, 0x0c0000, CRC(5cd273cd) SHA1(602e1f10454e2b1c941f2e6983872bb9ca77a542) )

	ROM_CONTINUE(             0x100000, 0x040000  )
	ROM_LOAD( "ux001014.u70", 0x140000, 0x080000, CRC(86b49b4e) SHA1(045b352950d848907af4c22b817d154b2cfff382) )
ROM_END

 /******************
 Mobile Suit Gundam  
*********************/

ROM_START( msgundams01 )
	ROM_REGION( 0x200000, "maincpu", 0 )  
	ROM_LOAD16_WORD_SWAP( "fa003002_ps01.u25",  0x000000, 0x080000, CRC(b06cc71a) SHA1(b5de10639458a569fe4789e3595fa4acd645dbf4) )
	ROM_LOAD16_WORD_SWAP( "fa001001.u20",  0x100000, 0x100000, CRC(fca139d0) SHA1(b56282c69f7ec64c697a48e42d59a2565401c032) )

	ROM_REGION( 0x400000, "gfx1", 0 )
	ROM_LOAD( "fa001008.u21",  0x000000, 0x200000, CRC(e7accf48) SHA1(dca9d53bc9cf0ecb661358d5a3f388c4ce9388e7) )
	ROM_LOAD( "fa001007.u22",  0x200000, 0x200000, CRC(793198a6) SHA1(45f53870e74b14126680d18dd58dbbe01a6ef509) )

	ROM_REGION( 0x100000, "gfx2", 0 )
	ROM_LOAD( "fa001006.u23",  0x000000, 0x100000, CRC(3b60365c) SHA1(bdf5a0b1b45eb75dbbb6725d1e5303716321aeb9) )

	ROM_REGION( 0x080000, "gfx3", 0 )  
	ROM_LOAD( "fa001005.u24",  0x000000, 0x080000, CRC(8cd7ff86) SHA1(ce7eb90776e21239f8f52e822c636143506c6f9b) )

	ROM_REGION( 0x100000, "x1snd", 0 )
	ROM_LOAD( "fa001004.u26",  0x000000, 0x100000, CRC(b965f07c) SHA1(ff7827cc80655465ffbb732d55ba81f21f51a5ca) )
ROM_END

 /*****
 Rezon  
********/

ROM_START( rezons01 )
	ROM_REGION( 0x200000, "maincpu", 0 )    
	ROM_LOAD16_BYTE( "us001001_ps01.u3",  0x000000, 0x020000, CRC(8ee3b85d) SHA1(f3ed438b680e0e0b97f27f38580951029d1f1be1) )
	ROM_LOAD16_BYTE( "us001002_ps01.u4",  0x000001, 0x020000, CRC(56562aff) SHA1(7e72d4a9b46e2aa033a20f2c8bfac4d304ed0428) )
	ROM_LOAD16_BYTE( "us001004.103", 0x100000, 0x020000, CRC(54871c7c) SHA1(2f807b15760b1e712fa69eee6f33cc8a36ee1c02) )
	ROM_LOAD16_BYTE( "us001003.102", 0x100001, 0x020000, CRC(1ac3d272) SHA1(0f19bc9c19e355dad5b463b0fa33127523bf141b) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "us001006.u64",  0x000000, 0x080000, CRC(a4916e96) SHA1(bfb63b72273e4fbf0843b3201bb4fddaf54909a7) )
	ROM_LOAD( "us001005.u63",  0x080000, 0x080000, CRC(e6251ebc) SHA1(f02a4c8373e33fc57e18e39f1b5ecff3f6d9ca9e) )

	ROM_REGION( 0x080000, "gfx2", 0 ) 
	ROM_LOAD( "us001007.u66",  0x000000, 0x080000, CRC(3760b935) SHA1(f5fe69f7e93c90a5b6c1dff236402b962821e33f) )

	ROM_REGION( 0x080000, "gfx3", 0 )  
	ROM_LOAD( "us001008.u68",  0x000000, 0x080000, CRC(0ab73910) SHA1(78e2c0570c5c6f5e1cdb2fbeae73376923127024) )

	ROM_REGION( 0x100000, "x1snd", 0 ) 
	ROM_LOAD16_WORD_SWAP( "us001009.u70",  0x000000, 0x100000, CRC(0d7d2e2b) SHA1(cfba19314ecb0a49ed9ff8df32cd6a3fe37ff526) )
ROM_END

ROM_START( rezonts01 )
	ROM_REGION( 0x200000, "maincpu", 0 )    
	ROM_LOAD16_BYTE( "us001001_ps01.u3",  0x000000, 0x020000, CRC(8ee3b85d) SHA1(f3ed438b680e0e0b97f27f38580951029d1f1be1) )
	ROM_LOAD16_BYTE( "rezon_1_p_ps01.u4", 0x000001, 0x020000, CRC(f52aa5a6) SHA1(dfb1909e1bbb5b9462cb0b37e17db542cc7204ec) )
	ROM_LOAD16_BYTE( "us001004.103", 0x100000, 0x020000, CRC(54871c7c) SHA1(2f807b15760b1e712fa69eee6f33cc8a36ee1c02) )
	ROM_LOAD16_BYTE( "us001003.102", 0x100001, 0x020000, CRC(1ac3d272) SHA1(0f19bc9c19e355dad5b463b0fa33127523bf141b) )

	ROM_REGION( 0x100000, "gfx1", 0 )
	ROM_LOAD( "us001006.u64",  0x000000, 0x080000, CRC(a4916e96) SHA1(bfb63b72273e4fbf0843b3201bb4fddaf54909a7) )
	ROM_LOAD( "us001005.u63",  0x080000, 0x080000, CRC(e6251ebc) SHA1(f02a4c8373e33fc57e18e39f1b5ecff3f6d9ca9e) )

	ROM_REGION( 0x080000, "gfx2", 0 )
	ROM_LOAD( "us001007.u66",  0x000000, 0x080000, CRC(3760b935) SHA1(f5fe69f7e93c90a5b6c1dff236402b962821e33f) )

	ROM_REGION( 0x080000, "gfx3", 0 ) 
	ROM_LOAD( "us001008.u68",  0x000000, 0x080000, CRC(0ab73910) SHA1(78e2c0570c5c6f5e1cdb2fbeae73376923127024) )

	ROM_REGION( 0x100000, "x1snd", 0 )
	ROM_LOAD16_WORD_SWAP( "us001009.u70",  0x000000, 0x100000, CRC(0d7d2e2b) SHA1(cfba19314ecb0a49ed9ff8df32cd6a3fe37ff526) )
ROM_END

/*    YEAR  NAME           PARENT    MACHINE     INPUT     INIT            MONITOR COMPANY         FULLNAME FLAGS */
// The FairyLand Story
GAME( 200?, blandias01,   blandia,   blandia,   blandia,   seta_state,     init_blandia,   ROT0,   "Unknown",  "Blandia (Boss With Simple Attack Edition)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 200?, blandias02,   blandia,   blandia,   blandia,   seta_state,     init_blandia,   ROT0,   "DDJ",  "Blandia (Choice Hidden Character)", MACHINE_IMPERFECT_GRAPHICS )
GAME( 200?, blandias03,   blandia,   blandia,   blandia,   seta_state,     init_blandia,   ROT0,   "DDJ",  "Blandia (Easy Move)", MACHINE_IMPERFECT_GRAPHICS )
// Mobile Suit Gundam
GAME( 200?, msgundams01,  msgundam,  msgundam,  msgundam,  seta_state,     empty_init,     ROT0,   "DDJ",   "Mobile Suit Gundam (Easy Move)", 0 )
// Rezon
GAME( 200?, rezons01,     rezon,     rezon,     rezon,     seta_state,     init_rezon,     ROT0,   "Unknown",  "Rezon (3 button version (Fixed Vaws, Rapid Fire))", 0 )
GAME( 200?, rezonts01,    rezon,     rezon,     rezont,    seta_state,     init_rezon,     ROT0,   "Unknown",  "Rezon (3 button version (Fixed Vaws, Rapid Fire))(Taito)", 0 )