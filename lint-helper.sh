#!/usr/bin/env sh

USAGE="Usage: $0 [-c [format|lint]] [-a [format|lint|all]]"
STARTDIR=$(dirname "$0")
FORMAT_PATHS="$(echo "$STARTDIR"/src/*.cpp) $(echo "$STARTDIR"/lib/*.cpp) $(echo "$STARTDIR"/include/*.hpp)"

main() {
	if [ "$1" = "format" ]; then
		assert_exists "clang-format"

		# shellcheck disable=SC2086
		clang-format $FORMAT_ARGS $FORMAT_PATHS
	elif [ "$1" = "lint" ]; then
		assert_exists "clang-tidy"

		# shellcheck disable=SC2086
		clang-tidy $TIDY_ARGS $FORMAT_PATHS
	elif [ "$1" = "all" ]; then
		main "format"
		main "lint"
	fi
}

check_opts() {
	while getopts ":c:a:h" opt; do
		case "${opt}" in
			c)
				case "$OPTARG" in
					format|lint)
						FORMAT_ARGS="--verbose --dry-run --Werror"
						TIDY_ARGS=""
						ACTION="$OPTARG"
						;;
					*)
						fail_msg "$USAGE"
						;;
				esac
				;;
			a)
				case "$OPTARG" in
					format|lint|all)
						FORMAT_ARGS="--verbose -i"
						TIDY_ARGS="--fix"
						ACTION="$OPTARG"
						;;
					*)
						fail_msg "$USAGE"
						;;
				esac
				;;
			h)
				printf "%s\n" "$USAGE" && exit ;;
			*)
				fail_msg "$USAGE" ;;
		esac

		main "$ACTION"
	done

	shift $((OPTIND-1)) # unnecessary?
}

assert_exists() {
	command -v "$1" >/dev/null \
		|| fail_msg "No $1??"
}

fail_msg() {
	printf "%s\n" "$1" >&2
	exit 1
}

check_opts "$@"
