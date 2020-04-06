start_server {tags {"tracking"}} {
    # Create a deferred client we'll use to redirect invalidation
    # messages to.
    set rd1 [redis_deferring_client]
    $rd1 client id
    set redir [$rd1 read]
    $rd1 subscribe __redis__:invalidate
    $rd1 read ; # Consume the SUBSCRIBE reply.

    test {Clients are able to enable tracking and redirect it} {
        r CLIENT TRACKING on REDIRECT $redir
    } {*OK}

    test {The other connection is able to get invalidations} {
        r SET a 1
        r GET a
        r INCR a
        r INCR b ; # This key should not be notified, since it wasn't fetched.
        set keys [lindex [$rd1 read] 2]
        assert {[llength $keys] == 1}
        assert {[lindex $keys 0] eq {a}}
    }

    test {The client is now able to disable tracking} {
        # Make sure to add a few more keys in the tracking list
        # so that we can check for leaks, as a side effect.
        r MGET a b c d e f g
        r CLIENT TRACKING off
    }

    test {Clients can enable the BCAST mode with the empty prefix} {
        r CLIENT TRACKING on BCAST REDIRECT $redir
    } {*OK*}

    test {The connection gets invalidation messages about all the keys} {
        r MSET a 1 b 2 c 3
        set keys [lsort [lindex [$rd1 read] 2]]
        assert {$keys eq {a b c}}
    }

    test {Clients can enable the BCAST mode with prefixes} {
        r CLIENT TRACKING off
        r CLIENT TRACKING on BCAST REDIRECT $redir PREFIX a: PREFIX b:
        r MULTI
        r INCR a:1
        r INCR a:2
        r INCR b:1
        r INCR b:2
        r EXEC
        # Because of the internals, we know we are going to receive
        # two separated notifications for the two different prefixes.
        set keys1 [lsort [lindex [$rd1 read] 2]]
        set keys2 [lsort [lindex [$rd1 read] 2]]
        set keys [lsort [list {*}$keys1 {*}$keys2]]
        assert {$keys eq {a:1 a:2 b:1 b:2}}
    }
    
    test {Adding prefixes to BCAST mode works} {
        r CLIENT TRACKING on BCAST REDIRECT $redir PREFIX c:
        r INCR c:1234
        set keys [lsort [lindex [$rd1 read] 2]]
        assert {$keys eq {c:1234}}
    }

    $rd1 close
}
