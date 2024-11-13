package(default_visibility=['//visibility:private'])

cc_library(
    name='anet',
    visibility=['//visibility:public'],
    srcs=glob(['*.cpp']),
    hdrs=glob(['*.h']),
    deps=['//agent_engine/alog:alog', '//agent_engine/util:env_util'],
    copts=['-Wformat-truncation=0'],
    alwayslink=True
)
