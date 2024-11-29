DIR=$(dirname "$0")

bash $DIR/build.sh

pass_total=0
fail_total=0

for test in $DIR/tests/should_compile_*; do
  test_basename=$(basename "$test")

  if [ "$1" = "--silent" ]; then
    $DIR/build/tl13 < $DIR/tests/$test_basename > /dev/null
  else
    $DIR/build/tl13 < $DIR/tests/$test_basename
  fi

  if [ $? -eq 0 ]; then
      echo "PASS... $test_basename"

      pass_total=$(expr $pass_total + 1)
  else
      echo "FAIL... $test_basename"

      fail_total=$(expr $fail_total + 1)
  fi
done

for test in $DIR/tests/should_not_compile_*; do
  test_basename=$(basename "$test")

  if [ "$1" = "--silent" ]; then
    $DIR/build/tl13 < $DIR/tests/$test_basename > /dev/null
  else
    $DIR/build/tl13 < $DIR/tests/$test_basename
  fi

  if ! [ $? -eq 0 ]; then
      echo "PASS... $test_basename"

      pass_total=$(expr $pass_total + 1)
  else
      echo "FAIL... $test_basename"

      fail_total=$(expr $fail_total + 1)
  fi
done

echo ""
echo "$pass_total PASSED, $fail_total FAILED"
