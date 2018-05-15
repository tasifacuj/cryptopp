#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T18:02:35
#
#-------------------------------------------------

QT       -= gui core

TARGET = cryptopp
TEMPLATE = lib

gcc:QMAKE_CXXFLAGS += -msha -march=native

CONFIG += c++14


CONFIG += staticlib
CONFIG -= debug_and_release debug_and_release_target

DEFINES -= UNICODE

CONFIG(release, debug|release) {
  BuildConfig = "release"
}
CONFIG(debug, debug|release) {
  BuildConfig = "debug"
}

DESTDIR = $${PWD}/bin/$${BuildConfig}
OBJECTS_DIR = $${PWD}/intermediate/$${BuildConfig}
MOC_DIR = $${PWD}/intermediate/$${BuildConfig}
RCC_DIR = $${PWD}/intermediate/$${BuildConfig}
UI_DIR  = $${PWD}/intermediate/$${BuildConfig}
SUBDIRS = $${PWD}/intermediate/$${BuildConfig}

SOURCES += \
    3way.cpp \
    adler32.cpp \
    algebra.cpp \
    algparam.cpp \
    arc4.cpp \
    aria-simd.cpp \
    aria.cpp \
    ariatab.cpp \
    asn.cpp \
    authenc.cpp \
    base32.cpp \
    base64.cpp \
    basecode.cpp \
    bench1.cpp \
    bench2.cpp \
    bfinit.cpp \
    blake2-simd.cpp \
    blake2.cpp \
    blowfish.cpp \
    blumshub.cpp \
    camellia.cpp \
    cast.cpp \
    casts.cpp \
    cbcmac.cpp \
    ccm.cpp \
    chacha.cpp \
    channels.cpp \
    cmac.cpp \
    cpu.cpp \
    crc-simd.cpp \
    crc.cpp \
    cryptlib.cpp \
    datatest.cpp \
    default.cpp \
    des.cpp \
    dessp.cpp \
    dh.cpp \
    dh2.cpp \
    dll.cpp \
    dlltest.cpp \
    dsa.cpp \
    eax.cpp \
    ec2n.cpp \
    eccrypto.cpp \
    ecp.cpp \
    elgamal.cpp \
    emsa2.cpp \
    eprecomp.cpp \
    esign.cpp \
    files.cpp \
    filters.cpp \
    fips140.cpp \
    fipsalgt.cpp \
    fipstest.cpp \
    gcm-simd.cpp \
    gcm.cpp \
    gf256.cpp \
    gf2n.cpp \
    gf2_32.cpp \
    gfpcrypt.cpp \
    gost.cpp \
    gzip.cpp \
    hex.cpp \
    hmac.cpp \
    hrtimer.cpp \
    ida.cpp \
    idea.cpp \
    integer.cpp \
    iterhash.cpp \
    kalyna.cpp \
    kalynatab.cpp \
    keccak.cpp \
    luc.cpp \
    mars.cpp \
    marss.cpp \
    md2.cpp \
    md4.cpp \
    md5.cpp \
    misc.cpp \
    modes.cpp \
    mqueue.cpp \
    mqv.cpp \
    nbtheory.cpp \
    neon-simd.cpp \
    network.cpp \
    oaep.cpp \
    osrng.cpp \
    padlkrng.cpp \
    panama.cpp \
    pch.cpp \
    pkcspad.cpp \
    poly1305.cpp \
    polynomi.cpp \
    ppc-simd.cpp \
    pssr.cpp \
    pubkey.cpp \
    queue.cpp \
    rabin.cpp \
    randpool.cpp \
    rc2.cpp \
    rc5.cpp \
    rc6.cpp \
    rdrand.cpp \
    rdtables.cpp \
    regtest1.cpp \
    regtest2.cpp \
    regtest3.cpp \
    rijndael-simd.cpp \
    rijndael.cpp \
    ripemd.cpp \
    rng.cpp \
    rsa.cpp \
    rw.cpp \
    safer.cpp \
    salsa.cpp \
    scrypt.cpp \
    seal.cpp \
    seed.cpp \
    serpent.cpp \
    sha-simd.cpp \
    sha.cpp \
    sha3.cpp \
    shacal2-simd.cpp \
    shacal2.cpp \
    shark.cpp \
    sharkbox.cpp \
    simon-simd.cpp \
    simon.cpp \
    simple.cpp \
    skipjack.cpp \
    sm3.cpp \
    sm4.cpp \
    socketft.cpp \
    sosemanuk.cpp \
    speck-simd.cpp \
    speck.cpp \
    square.cpp \
    squaretb.cpp \
    sse-simd.cpp \
    strciphr.cpp \
    tea.cpp \
    test.cpp \
    tftables.cpp \
    threefish.cpp \
    tiger.cpp \
    tigertab.cpp \
    trdlocal.cpp \
    ttmac.cpp \
    tweetnacl.cpp \
    twofish.cpp \
    validat0.cpp \
    validat1.cpp \
    validat2.cpp \
    validat3.cpp \
    validat4.cpp \
    vmac.cpp \
    wait.cpp \
    wake.cpp \
    whrlpool.cpp \
    winpipes.cpp \
    xtr.cpp \
    xtrcrypt.cpp \
    zdeflate.cpp \
    zinflate.cpp \
    zlib.cpp

HEADERS +=\
    3way.h \
    adler32.h \
    adv-simd.h \
    aes.h \
    algebra.h \
    algparam.h \
    arc4.h \
    argnames.h \
    aria.h \
    asn.h \
    authenc.h \
    base32.h \
    base64.h \
    basecode.h \
    bench.h \
    blake2.h \
    blowfish.h \
    blumshub.h \
    camellia.h \
    cast.h \
    cbcmac.h \
    ccm.h \
    chacha.h \
    channels.h \
    cmac.h \
    config.h \
    cpu.h \
    crc.h \
    cryptlib.h \
    default.h \
    des.h \
    dh.h \
    dh2.h \
    dll.h \
    dmac.h \
    drbg.h \
    dsa.h \
    eax.h \
    ec2n.h \
    eccrypto.h \
    ecp.h \
    ecpoint.h \
    elgamal.h \
    emsa2.h \
    eprecomp.h \
    esign.h \
    factory.h \
    fhmqv.h \
    files.h \
    filters.h \
    fips140.h \
    fltrimpl.h \
    gcm.h \
    gf256.h \
    gf2n.h \
    gf2_32.h \
    gfpcrypt.h \
    gost.h \
    gzip.h \
    hashfwd.h \
    hex.h \
    hkdf.h \
    hmac.h \
    hmqv.h \
    hrtimer.h \
    ida.h \
    idea.h \
    integer.h \
    iterhash.h \
    kalyna.h \
    keccak.h \
    lubyrack.h \
    luc.h \
    mars.h \
    md2.h \
    md4.h \
    md5.h \
    mdc.h \
    mersenne.h \
    misc.h \
    modarith.h \
    modes.h \
    modexppc.h \
    mqueue.h \
    mqv.h \
    naclite.h \
    nbtheory.h \
    network.h \
    nr.h \
    oaep.h \
    oids.h \
    osrng.h \
    ossig.h \
    padlkrng.h \
    panama.h \
    pch.h \
    pkcspad.h \
    poly1305.h \
    polynomi.h \
    ppc-simd.h \
    pssr.h \
    pubkey.h \
    pwdbased.h \
    queue.h \
    rabin.h \
    randpool.h \
    rc2.h \
    rc5.h \
    rc6.h \
    rdrand.h \
    resource.h \
    rijndael.h \
    ripemd.h \
    rng.h \
    rsa.h \
    rw.h \
    safer.h \
    salsa.h \
    scrypt.h \
    seal.h \
    secblock.h \
    seckey.h \
    seed.h \
    serpent.h \
    serpentp.h \
    sha.h \
    sha3.h \
    shacal2.h \
    shark.h \
    simon.h \
    simple.h \
    siphash.h \
    skipjack.h \
    sm3.h \
    sm4.h \
    smartptr.h \
    socketft.h \
    sosemanuk.h \
    speck.h \
    square.h \
    stdcpp.h \
    strciphr.h \
    tea.h \
    threefish.h \
    tiger.h \
    trap.h \
    trdlocal.h \
    trunhash.h \
    ttmac.h \
    tweetnacl.h \
    twofish.h \
    validate.h \
    vmac.h \
    wait.h \
    wake.h \
    whrlpool.h \
    winpipes.h \
    words.h \
    xtr.h \
    xtrcrypt.h \
    zdeflate.h \
    zinflate.h \
    zlib.h
